/*EEPROM读取函数*/
/*addr：地址；number：长度；p_buff：读出数据存放指针*/
void eprom_read(unsigned int addr, unsigned char number, unsigned char *p_buff) {
    CLI();
	while(EECR & (1 << EEWE));
	EEARH = 0x00;
	while(number --) {
		EEARL = addr ++;
		EECR |= (1 << EERE);
		*p_buff++ = EEDR;
		}
    SEI();
}

/*EEPROM写入函数*/
/*addr：地址；number：长度；p_buff：写入数据存放指针*/
void eprom_write(unsigned int addr, unsigned char number, unsigned char *p_buff) {
    CLI();
	EEARH = 0x00;
	while(number --) {
		while(EECR & (1 << EEWE));
		EEARL = addr ++;
		EEDR = *p_buff ++;
		EECR |= (1 << EEMWE);
		EECR |= (1 << EEWE);
		}
    SEI();
}