/*******************************************************************************
 * UART Common Library
 *******************************************************************************/
const uns8 hex2char[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
void putch(uns8 byte) 
{
							/* output one byte */
	while(!TXIF)			/* set when register is empty */
		continue;
	TXREG = byte;
}
uns8 getch() {
							/* retrieve one byte */
	while(!RCIF)			/* set when register is not empty */
		continue;
	return RCREG;	
}
void writeCString(const uns8 *s)
{
	char u;
	while(1){
		u = *s++;
        if (!u)
            break;
		putch(u);
	}
}
void writeUNS8(uns8 v){
	uns8 i;
	i = v >>4;
	putch(hex2char[i]);		//high 4-bit
	i = v & 0x0f;
	putch(hex2char[i]);		//low 4-bit
}
#define CR()	putch(0x0D);
#define LF()	putch(0x0A);
void crlf(void){
	CR();
	LF();
}