void DingShiQi_TC1(void)
{
    TCCR1B=0X03;//设置TC1 为 8M/64分频=125000，启动TC1
    TCNT1H=0XFF;//设置 TC1 的输入捕获寄存器 高8位值
    TCNT1L=0X82;//设置 TC1 的输入捕获寄存器 低8位值
    TIMSK|=BIT(2);//设置 定时计数器 的 屏蔽寄存器
    SREG|=BIT(7);//使用中断时必须先开启总中断
}