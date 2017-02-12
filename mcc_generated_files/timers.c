void initTmr1()
{
 TMR1 = 0;			// clear timer 1
 PR1 = 0x7FFF;
 IFS0bits.T1IF = 0;		// clr interrupt flag
 IEC0bits.T1IE = 1;		// set interrupt enable bit
 T1CON = 0x8030;                // Timer1 on with prescaler
}


