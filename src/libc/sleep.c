unsigned int
sleep(seconds)
	unsigned int seconds;
{
	volatile unsigned int i;
	for (i=0; i<4000000*seconds; i++) {} /* good for about 12MHz */
	return(seconds);
}
