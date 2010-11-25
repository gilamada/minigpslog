#pragma interrupt_handler SIG_ADC:15 //声明中断函数为miao 中断向量号为9
#define MaxAdcBufSize 8
static volatile unsigned int ADCBuffer[MaxAdcBufSize];


/********************************************************
ADC初始化
********************************************************/
void InitADC(void)
{ 
	ADMUX |= (0<<REFS1)|(0<<REFS0);  //内部Vref 关闭
	ADCSRA |= (1<<ADEN)|(1<<ADSC)|(1<<ADATE)|(1<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(0<<ADPS0);  //ADC使能;ADC开始转换;连续转换;中断使能;64分频
}


/********************************************************
读取ADC
********************************************************/
unsigned int ReadADC(void)
{ 
	return ADCBuffer[0];
}


/********************************************************
ADC转换完成中断
********************************************************/
void SIG_ADC(void)
{ 
	unsigned char i;
	unsigned int tmp,sum = 0;
	tmp = ADC;
	
	for(i=1;i<MaxAdcBufSize - 1;i++)
	{
		ADCBuffer[i] = ADCBuffer[i+1];
		sum += ADCBuffer[i];
	}
	ADCBuffer[MaxAdcBufSize - 1] = tmp;
	sum += ADCBuffer[MaxAdcBufSize - 1];
	ADCBuffer[0] = sum / (MaxAdcBufSize - 1);
}