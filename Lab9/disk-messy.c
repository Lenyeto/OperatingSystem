
#include "kprintf.h"

//https://github.com/torvalds/linux/blob/master/drivers/mmc/host/dw_mmc.h
//	gives bit assignments for some registers
//https://github.com/torvalds/linux/blob/master/drivers/mmc/host/mmci.c
//https://github.com/torvalds/linux/blob/master/drivers/mmc/host/mmci.h
//	driver for pl181 in linux
//https://www.sdcard.org/developers/overview/host_controller/simple_spec/index.html
//http://www.jedec.org/standards-documents/results/jesd84-b51
//http://infocenter.arm.com/help/topic/com.arm.doc.ddi0172a/DDI0172.pdf
//https://www.sdcard.org/downloads/pls/simplified_specs/archive/part1_101.pdfs
//https://www.sdcard.org/developers/overview/host_controller/simple_spec/Simplified_SD_Host_Controller_Spec.pdf
//http://www.sandisk.com/media/File/OEM/Manuals/SD_SDIO_specsv1.pdf
//http://wiki.seabright.co.nz/wiki/SdCardProtocol.html
//http://www.eecs.wsu.edu/~cs460/sdcDriver.html
const char* msgs[]={
    "CmdCRCFail","DataCRCFail", "CmdTimeout", "DataTimeout",
    "TransUnder","RecvUnder","CmdRespEnd","Sent",
    "DataEnd","StartErr","DataBlockEnd","CmdActive",
    "Transmitting","Recving", "TransHalfempty",
    "RecvHalffull",  "TransFull","RecvFull",
    "Transempty","Recvempty","TransData","RecvData"};
 
void statusflags(int x){
    int i;
    for(i=0;i<22;++i){
        if( x & (1<<i) )
            kprintf("%s ",msgs[i]);
    }
    return;
}

#pragma pack(push,1)
struct MMC{
    unsigned power;
    unsigned clock;
    unsigned arg;
    unsigned cmd;
    unsigned response_cmd;
    unsigned response[4];
    unsigned data_timer;
    unsigned data_length;
    unsigned data_control;
    unsigned data_counter;
    unsigned status;
    unsigned clear;
    unsigned interrupt0_mask;
    unsigned interrupt1_mask;
    unsigned select;
    unsigned fifo_count;
    unsigned reserved[13];
    unsigned data_fifo[16];
};
#pragma pack(pop)

volatile struct MMC* mmcp = (struct MMC*)0x1c000000;
volatile unsigned* mmc = (unsigned*) 0x1c000000;

int isBusy(){
    //return busy bit
    return mmcp->status & (1<<24) ; 
}

void disk_init(){
    
    //mmc test
    //IRQ 23 and 24
    //pl181 mmc interface
    //unmarked are rw  
    //power is +0 (rw)
    //clock is +4 (rw)  [1]
    //arg is +8 (rw)  [2]
    //cmd is +12 (rw) [3]
    //response cmd is +16 0x10(r)  [4]
    //response is +20,+24,+28,+32 (r)  [5][6][7][8]
    //data timer is +36 0x24  [9]
    //data length is +40 [10]
    //data control +44 0x2c [11]
    //data counter +48(r) 0x30 [12]
    //status +52(r) [13]
    //clear +56(write only) [14]
    //interrupt 0 mask +60  [15]
    //interrupt 1 mask 0x40 [16]
    //select [17]
    //fifo count (r) [18]
    //reserved stuff
    //Data fifo is at 0x1c000080-0x1c0000bc = offset 0x80 = [32]...[47]
    //fe0-fec are id registers (r)
    //likewise for ff0-ffc (primecell id registers) (r)
    
    //see if it's busy:
    //read status reg, look at bit 25
    
    //commands:
    //  bits 0-5 = command
    //  6=wait for a response
    //  7=wait for 136 bit response
    //  8=wait for interrupt
    //  9=wait for CmdPend and then send command
    //  10=enable CPSM (command path state machine)
    
    
    //commands, with response types
    //  0: reset: 1
    //  10: send card id: 1
    //  17: read one block. param = block num: 1
    //  24: write one block param = block num: 1
    //  16: set block len. param=block size. Fixed at 512 for sdhc/sdxc: 1
    
    //power up the mmc
    mmcp->power = 3;
    
    //enable clock
    mmcp->clock = 8;
    
    //reset
    mmcp->cmd = (1<<10);

    //the mmc is now in state idle_state, mode card_identification_mode

    //activate 3.3v parts. QEMU doesn't seem to care, but 
    //it does have some logic for handling this.
    mmcp->cmd = 8 | (1<<10);


    //go active
    
    //we must repeat these until the card is not busy
    do{
        //55 = app specific command is next
        mmcp->cmd = 55 | (1<<10) | (1<<6);
        
        if( mmcp->response[0] != 0x120 )
            kprintf("Uh oh.");
            
        //set argument
        mmcp->arg = 0xffffffff; 	//delay. Not relevant, but must be > 0
        mmcp->cmd = 41 | (1<<10) | (1<<6);        //41 = send_op_cond 
        
    } while( isBusy() );
    
    //we are now in ready state, card identification mode
    
    //Request the MMC to send its CardID
    //move to identification state. This gives a long response.
    mmcp->cmd = 2 | (1<<10) | (1<<6) | (1<<7);
   
    //we are now in identification state
    //we can get to standby state by using cmd3 Send relative address
    mmcp->cmd = 3 | (1<<10) | (1<<6) ;

    unsigned relative_address = mmcp->response[0];
    
    //select the card. Docs seem to indicate there
    //is no response from this one. But Qemu
    //seems to think there is.
    mmcp->arg = relative_address;
    mmcp->cmd = 7 | (1<<10) | (1<<6); 
    
    //could get interrupts: base+MASK0 = (1<<21) | (1<<18) for rx avail/tx empty
}

void disk_read_sector(unsigned sector, char* datablock){

    //time to read a block
    mmcp->data_length = 512;
    mmcp->data_timer = 100;	//timeout. What should this be?
    
    //bit 0: 1=data transfer enabled
    //bit 1: 1=read, 0=write
    //bit 2: 0=block, 1=stream
    //bit 3: 0=no dma, 1=dma
    //bits 4-7: block size: Power of 2. Ex: 11=2**11
    mmcp->data_control = (1<<0) | (1<<1) | (9<<4);
    
    //looks like qemu emulates SD, not SDHC.
    //block number. Or is it byte number? Depends on SD vs SDHC.
    //Must be multiple of 512 if byte number.
    mmcp->arg = 512*sector;     
    
    mmcp->clear=0xffff; //clear status flags
    mmcp->cmd = 17 | (1<<10) | (1<<6);  //read!
    
    unsigned* p = (unsigned*) datablock;
    int k;
    
    //get 4 bytes at a time...
    for(k=0;k<128;++k){
        //wait for data to be ready
        while( !(mmcp->status & (1<<21)) )
            ;	
        mmcp->clear = 0xffff;
        unsigned v = mmcp->data_fifo[0];
        *p++ = v;
    }
    
    //FIXME: send cmd12 when done
    
}
    
void disk_write_sector(unsigned sector, const char* datablock){
    mmcp->data_length = 512; //data length
    mmcp->data_timer = 100;   //timeout. What should this be?
    mmcp->data_control = (1<<0) | (0<<1) | (9<<4);
    mmcp->arg = 512*sector;     
    mmcp->clear=0xffff; //clear status flags
    mmcp->cmd = 24 | (1<<10) | (1<<6); 	//do the write
    unsigned* p = (unsigned*) datablock;
    int k;
    for(k=0;k<128;++k){
        //wait for transmit fifo to be empty
        while( (mmcp->status & (1<<20)) )
            ;
        mmcp->clear = 0xffff;
        mmcp->data_fifo[0] = *p;
        p++;
    }
 
    //FIXME: send cmd12 when done
}

