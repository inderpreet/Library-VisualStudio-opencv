
float Filter1(char c)
{
	float temp = 448.540;
	if(c == 'z' || c=='x' || c== 's')			// sedan
		temp = 452.385;
	else if(c =='f')						// fiat
		temp = 418.870;
	else if(c == 'v' ||  c== 'b' || c == 'i')	// Indica
		temp = 368.238;
	else if(c == 'n')
		temp = 306.708;
	else
		temp = 0;
	return temp;
}