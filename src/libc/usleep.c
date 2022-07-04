void
usleep(useconds)
	unsigned int useconds;
{
	volatile unsigned int i;
	for (i=0; i<4*useconds; i++) {} /* good for about 12MHz */
}
