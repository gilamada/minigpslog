/*这份代码经源码格式软件格式化过
     yang_hx@neusoft.com      */
/*********************************************************************/
/*                     USART_PRINTP for STM32
                                                                     */
/*********************************************************************/
/*********************************************************************/

/* printp() ------------ a simply version of printp()                */

/**********************************************************************/
/* with the help of printp() function, you can get the out format as below:
   printp("This first line outputs the format string.\n");
   printp("%s\n","This second line outputs a string.");
   printp( "Notice that the first %s\n","two output lines had CR/LF at the end.");
   printp( "%c%c%c%c line outputs 'This' as chars.\n",'T', 'h', 'i', 's');
   printp( "Dec values: %d  %d   Hex value: %x\n",dec_value, neg_value, hex_value);
   printp( "Long dec values: %ld  %ld\n",ldec_value,lneg_value);
   printp( "Output sized strings: %20s %-20s\n","Right string","Left string");
   printp( "Output sized values:  %20d %-20d\n",dec_value,neg_value);
   printp( "                      %20x %-20x\n",hex_value,hex_value);
   printp( "                      %20ld %-20ld\n",ldec_value,ldec_value);
where the variables can be as:
   int hex_value = 0x1234;
   int dec_value = 1234;
   int neg_value = -1234;
   long ldec_value = 12345678L;
   long lneg_value = -12345678L;


修改了部分printp()代码，
 打印时如果 %后面的x,d为大写，打印的数不区分正负数
                      为小写，区分正负数
                 加l为取长型变量。

例如打印如下语句：

	printp("\n here i is 0xF123ABCD, j is 0xF123");
	printp("\n 1 : %%lX i is: %lX",i);
	printp("\n 2 : %%lx i is: %lx",i);
	printp("\n 3 : %%lD i is: %lD",i);
	printp("\n 4 : %%ld i is: %ld",i);
	printp("\n 5 : %%8X i is: %8X",i);
	printp("\n 6 : %%8x i is: %8x",i);
	printp("\n 7 : %%8D i is: %8D",i);
	printp("\n 8 : %%8d i is: %8d",i);
	printp("\n 9 :  %%X j is: %X",j);
	printp("\n 10:  %%x j is: %x",j);
	printp("\n 11:  %%D j is: %D",j);
	printp("\n 12:  %%d j is: %d",j);
结果为：
	 here i is 0xF123ABCD, j is 0xF123
	 1 : %lX i is: F123ABCD
	 2 : %lx i is: -EDC5433
	 3 : %lD i is: 4045646797
	 4 : %ld i is: -249320499
	 5 : %8X i is:     F123
	 6 : %8x i is:     -EDD
	 7 : %8D i is:    61731
	 8 : %8d i is:    -3805
	 9 :  %X j is: F123
	 10:  %x j is: -EDD
	 11:  %D j is: 61731
	 12:  %d j is: -3805  */

/************************************************************************/
#include "Config.h" 

void putchar1(INT8U ch)
{
    /* Write a character to the USART */
    USART_SendData(USART1,(INT8U)ch);
    
    /* Loop until the end of transmission */
    while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET){}
}

INT8U getchar1(void)
{
    /* Loop until RXNE = 1 */
   USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);	//关闭串口中断  崔超更改

    while(USART_GetFlagStatus(USART1,USART_FLAG_RXNE)==RESET){}
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	//打开串口中断  崔超更改 
	
	return(USART_ReceiveData(USART1));



}

//格式转换函数，十六进制转换为字符串  eg:输入0x5A，输出5A
void PUT_INT8U_HexTOString(INT8U Hex)
{
    INT8U temp1,temp2 ;
    
    temp1=(Hex>>4)&0x0F ;
    temp2=Hex&0x0F ;
    
    if(temp1<10)
	    putchar1(temp1+'0');
    else 
        putchar1(temp1-10+'A');
    
    if(temp2<10)
        putchar1(temp2+'0');
    else 
        putchar1(temp2-10+'A');
}

//格式转换函数，十进制数据转换为字符串 eg：输入001234，串口输出1234(去掉无效0)
void PUT_INT32U_DecTOString(INT32U Dec)
{
    INT8S temp8S ;
    INT32U temp32U ;
    char buffer[12];
    
    temp32U=Dec ;
    for(temp8S=10;temp8S>=0;temp8S--)
    {
        buffer[temp8S]=temp32U%10+'0' ;
        temp32U=temp32U/10 ;
    }
    buffer[11]='\0' ;
    
    if(buffer[0]=='0')
    {
        if(buffer[1]=='0')
        {
            if(buffer[2]=='0')
            {
                if(buffer[3]=='0')
                {
                    if(buffer[4]=='0')
                    {
                        if(buffer[5]=='0')
                        {
                            if(buffer[6]=='0')
                            {
                                if(buffer[7]=='0')
                                {
                                    if(buffer[8]=='0')
                                    {
                                        if(buffer[9]=='0') printp(&buffer[10]);
                                        else printp(&buffer[9]);
                                    }
                                    else printp(&buffer[8]);
                                }
                                else printp(&buffer[7]);
                            }
                            else printp(&buffer[6]);
                        }
                        else printp(&buffer[5]);
                    }
                    else printp(&buffer[4]);
                }
                else printp(&buffer[3]);
            }
            else printp(&buffer[2]);
        }
        else printp(&buffer[1]);
    }
    else printp(&buffer[0]);
}

//串口输入字符，转换成十进制，即"123456"--123456
INT32U StringTODec(void)
{
    INT8U num,temp8U ;
    INT32U temp32U ;
    
    temp8U=0x00 ;
    temp32U=0x00 ;
    num=0x00 ;
    
    //如果不是回车键
    while(temp8U!=0x0D)
    {
        temp8U=getchar1();
        
        if(temp8U>='0'&&temp8U<='9')
        {
            putchar1(temp8U);
            num++;
            switch(num)
            {
                case 1 :
                temp32U=(INT32U)(temp8U-'0');
                break ;
                case 2 :
                temp32U=temp32U*10+(INT32U)(temp8U-'0');
                break ;
                case 3 :
                temp32U=temp32U*10+(INT32U)(temp8U-'0');
                break ;
                case 4 :
                temp32U=temp32U*10+(INT32U)(temp8U-'0');
                break ;
                case 5 :
                temp32U=temp32U*10+(INT32U)(temp8U-'0');
                break ;
                case 6 :
                temp32U=temp32U*10+(INT32U)(temp8U-'0');
                break ;
                case 7 :
                temp32U=temp32U*10+(INT32U)(temp8U-'0');
                break ;
                case 8 :
                temp32U=temp32U*10+(INT32U)(temp8U-'0');
                num=0 ;
                putchar1(' ');
                break ;
                default :
                num=0 ;
                break ;
            }
        }
    }
    return temp32U ;
}

/* printp() ------------ a simply version of printp()                */
/* with the help of printp() function, you can get the out format as below:
   printp("This first line outputs the format string.\n");
   printp("%s\n","This second line outputs a string.");
   printp( "Notice that the first %s\n","two output lines had CR/LF at the end.");
   printp( "%c%c%c%c line outputs 'This' as chars.\n",'T', 'h', 'i', 's');
   printp( "Dec values: %d  %d   Hex value: %x\n",dec_value, neg_value, hex_value);
   printp( "Long dec values: %ld  %ld\n",ldec_value,lneg_value);
   printp( "Output sized strings: %20s %-20s\n","Right string","Left string");
   printp( "Output sized values:  %20d %-20d\n",dec_value,neg_value);
   printp( "                      %20x %-20x\n",hex_value,hex_value);
   printp( "                      %20ld %-20ld\n",ldec_value,ldec_value);

   where the variables can be as:
   int hex_value = 0x1234;
   int dec_value = 1234;
   int neg_value = -1234;
   long ldec_value = 12345678L;
   long lneg_value = -12345678L;

	printp("\n here i is 0xF123ABCD, j is 0xF123");
	printp("\n 1 : %%lX i is: %lX",i);
	printp("\n 2 : %%lx i is: %lx",i);
	printp("\n 3 : %%lD i is: %lD",i);
	printp("\n 4 : %%ld i is: %ld",i);
	printp("\n 5 : %%8X i is: %8X",i);
	printp("\n 6 : %%8x i is: %8x",i);
	printp("\n 7 : %%8D i is: %8D",i);
	printp("\n 8 : %%8d i is: %8d",i);
	printp("\n 9 :  %%X j is: %X",j);
	printp("\n 10:  %%x j is: %x",j);
	printp("\n 11:  %%D j is: %D",j);
	printp("\n 12:  %%d j is: %d",j);

	 here i is 0xF123ABCD, j is 0xF123
	 1 : %lX i is: F123ABCD
	 2 : %lx i is: -EDC5433
	 3 : %lD i is: 4045646797
	 4 : %ld i is: -249320499
	 5 : %8X i is:     F123
	 6 : %8x i is:     -EDD
	 7 : %8D i is:    61731
	 8 : %8d i is:    -3805
	 9 :  %X j is: F123
	 10:  %x j is: -EDD
	 11:  %D j is: 61731
	 12:  %d j is: -3805  */

/************************************************************************/



#ifdef PRINTP_EN 

static int do_padding ;
static int left_flag ;
static int len ;
static int num1 ;
static int num2 ;
static char pad_character ;

/*---------------------------------------------------*/
/*                                                   */
/* This routine puts pad characters into the output  */
/* buffer.                                           */
/*                                                   */
static void padding(const int l_flag)
{
    int i ;
    
    if(do_padding&&l_flag&&(len<num1))
    {
        for(i=len;i<num1;i++)putchar1(pad_character);
    }
}

/*---------------------------------------------------*/
/*                                                   */
/* This routine moves a string to the output buffer  */
/* as directed by the padding and positioning flags. */
/*                                                   */
static void outs(char*lp)
{
    /* pad on left if needed                          */
    len=strlen(lp);
    padding(!left_flag);
    
    /* Move string to the buffer                      */
    while(*lp&&num2--)putchar1(*lp++);
    
    /* Pad on right if needed                         */
    len=strlen(lp);
    padding(left_flag);
}

/*---------------------------------------------------*/
/*                                                   */
/* This routine moves a number to the output buffer  */
/* as directed by the padding and positioning flags. */
/*                                                   */
static void reoutnum(unsigned long num,unsigned int negative,const long base)
{
    char*cp ;
    char outbuf[32];
    const char digits[]="0123456789ABCDEF" ;
    
    /* Build number (backwards) in outbuf             */
    cp=outbuf ;
    do 
    {
        *cp++=digits[(int)(num%base)];
    }
    while((num/=base)>0);
    if(negative)
    *cp++='-' ;
    *cp--=0 ;
    
    /* Move the converted number to the buffer and    */
    /* add in the padding where needed.               */
    len=strlen(outbuf);
    padding(!left_flag);
    while(cp>=outbuf) putchar1(*cp--);
    padding(left_flag);
}


//1, signed  0 unsigned
static void outnum(long num,const long base,unsigned char sign)
{
    unsigned int negative ;
    if((num<0L)&&sign)
    {
        negative=1 ;
        num=-num ;
    }
    else negative=0 ;
    
    reoutnum(num,negative,base);
}
/*---------------------------------------------------*/
/*                                                   */
/* This routine gets a number from the format        */
/* string.                                           */
/*                                                   */
static int getnum(char**linep)
{
    int n ;
    char*cp ;
    
    n=0 ;
    cp=*linep ;
    while(isdigit(*cp)) n=n*10+((*cp++)-'0');
    
	*linep=cp ;
    return(n);
}

/*---------------------------------------------------*/
/*                                                   */
/* This routine operates just like a printp/sprintp  */
/* routine. It outputs a set of data under the       */
/* control of a formatting string. Not all of the    */
/* standard C format control are supported. The ones */
/* provided are primarily those needed for embedded  */
/* systems work. Primarily the floaing point         */
/* routines are omitted. Other formats could be      */
/* added easily by following the examples shown for  */
/* the supported formats.                            */
/*                                                   */

void printp(char*ctrl,...)
{
    
    int long_flag ;
    int dot_flag ;
    
    char ch ;
    va_list argp ;
    
    va_start(argp,ctrl);
    
    for(;*ctrl;ctrl++)
    {
        /* move format string chars to buffer until a  */
        /* format control is found.                    */
        if(*ctrl!='%')
        {
            putchar1(*ctrl);
            #if CR_as_CRLF==TRUE 
            if(*ctrl=='\n')putchar1('\r');
            #endif 
            continue ;
        }
        
        /* initialize all the flags for this format.   */
        dot_flag=0 ;
        long_flag=0 ;
        left_flag=0 ;
        do_padding=0 ;
        pad_character=' ';
        num2=32767 ;
        
        try_next :
        ch=*(++ctrl);
        
        if(isdigit(ch))
        {
            if(dot_flag)
            num2=getnum(&ctrl);
            else 
            {
                if(ch=='0')
                pad_character='0' ;
                
                num1=getnum(&ctrl);
                do_padding=1 ;
            }
            ctrl--;
            goto try_next ;
        }
        
        switch(tolower(ch))
        {
            case '%' :
            putchar1('%');
            continue ;
            
            case '-' :
            left_flag=1 ;
            break ;
            
            case '.' :
            dot_flag=1 ;
            break ;
            
            case 'l' :
            long_flag=1 ;
            break ;
            
            case 'd' :
            if(long_flag==1)
            {
                if(ch=='D')
                {
                    outnum(va_arg(argp,unsigned long),10L,0);
                    continue ;
                }
                /* ch == 'd' */
                else 
                {
                    outnum(va_arg(argp,long),10L,1);
                    continue ;
                }
            }
            else 
            {
                if(ch=='D')
                {
                    outnum(va_arg(argp,unsigned int),10L,0);
                    continue ;
                }
                /* ch == 'd' */
                else 
                {
                    outnum(va_arg(argp,int),10L,1);
                    continue ;
                }
            }
            
            case 'x' :
            // X ??? , x  ???
            if(long_flag==1)
            {
                if(ch=='X')
                {
                    outnum(va_arg(argp,unsigned long),16L,0);
                    continue ;
                }
                /* ch == 'x' */
                else 
                {
                    outnum(va_arg(argp,long),16L,1);
                    continue ;
                }
            }
            else 
            {
                if(ch=='X')
                {
                    outnum(va_arg(argp,unsigned int),16L,0);
                    continue ;
                }
                /* ch == 'x' */
                else 
                {
                    outnum(va_arg(argp,int),16L,1);
                    continue ;
                }
            }
            //continue;
            
            case 's' :
            outs(va_arg(argp,char*));
            continue ;
            
            case 'c' :
            putchar1(va_arg(argp,int));
            continue ;
            
            default :
            continue ;
        }
        goto try_next ;
    }
    va_end(argp);
}

/*******************************************************************************
* Function Name  : USART_Scanf_Name
* Description    : Gets Char values from the hyperterminal.
* Input          : None
* Output         : None
* Return         : Length
*******************************************************************************/
INT8U USART_Scanf_Name(char * str)
{	
    INT8U index = 0;
    while(1) 
    {
        /* Loop until RXNE = 1 */
        while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET){}
        
		str[index++] = (USART_ReceiveData(USART1));
	    printp("%c",str[index - 1]);
	    if(str[index - 1] == 13)//回车键
	    {  
	        index--;
			str[index] ='\0';//结束符号
	        return index;
        }
    }													
}

/*******************************************************************************
* Function Name  : USART_Scanf_Cmd
* Description    : Gets Char values from the hyperterminal.
* Input          : None
* Output         : None
* Return         : Length
*******************************************************************************/
INT8U USART_Scanf_Cmd(char * str)
{	
    INT8U index = 0;
    while(1) 
    {
        /* Loop until RXNE = 1 */
        while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET){}
        
		str[index++] = (USART_ReceiveData(USART1));
	    printp("%c",str[index - 1]);
    	if(str[index - 1] == 13 || str[index - 1] == 32)
	    {  
	        index--;
	        return index;
        }
    }
}

/*******************************************************************************
* Function Name  : Equal
* Description    : Compare the string A and B
* Input          : A-- one string,B-- anohter string,SizeB-- the size of string
* Output         : None
* Return         : 1-- Equal 0-- Not Equal
*******************************************************************************/
INT8U Equal(void* A, void* B, INT8U SizeA, INT8U SizeB)
{
	INT8U i, *a = A, *b = B;
	if(SizeA != SizeB) return 0;
	for(i = 0; i < SizeA; i++)
	{
		if(a[i] != b[i]) return 0;
	}
	return 1;
}


#endif 



