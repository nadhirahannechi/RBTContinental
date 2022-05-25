#include <iostream>
#include <string>
#include <cstring>
#include <stdint.h>
#include <stdio.h>
#include <sstream> 

using namespace std;



string mac_to_string(unsigned char * m)
{
    string RValue = "";
    char aux[2];
    int a = m[0];
    int b = m[1];
    int c = m[2];
    int d = m[3];
    int e = m[4];
    int f = m[5];
    sprintf(aux, "%.2X", a);
    RValue += string(aux);
    RValue+=":";
    sprintf(aux, "%.2X", b);
    RValue += string(aux);
    RValue+=":";
    sprintf(aux, "%.2X", c);
    RValue += string(aux);
    RValue+=":";
    sprintf(aux, "%.2X", d);
    RValue += string(aux);
    RValue+=":";
    sprintf(aux, "%.2X", e);
    RValue += string(aux);
    RValue+=":";
    sprintf(aux, "%.2X", f);
    RValue += string(aux);
    return RValue;
}

void string_to_mac(string m, unsigned char p[])
{   
    for(int i=0;i<6;++i)
    {
        unsigned char number =0;
        string hex_string = m.substr(0,2);
        //cout<<hex_string<<endl;
        m.erase(0, 3);
	    number = stoi(hex_string, 0, 16);
	    //cout<<number<<endl;
	    p[i] = number;
	    //printf("%d ",p[i]);
    }
}

std::string ipadress_to_string(uint8_t * address)
{
    return to_string((int) address[0])+ '.'+  to_string((int) address[1])+ '.'+  to_string((int) address[2])+ '.'+  to_string((int) address[3]);
}
void string_to_ipadress(std::string str,uint8_t * ipAdress)
{   
    std::stringstream s(str);
    int a,b,c,d; 
    char ch; 
    s >> a >> ch >> b >> ch >> c >> ch >> d;
    
    ipAdress[0]=a;
    ipAdress[1]=b;
    ipAdress[2]=c;
    ipAdress[3]=d;

}

unsigned short in_cksum(unsigned short *ptr, int nbytes)
{
	
    register long sum;
	short int oddbyte;
	register short int answer;

	sum = 0;
	while (nbytes > 1) {
		sum += *ptr++;
		nbytes -= 2;
	}

	if (nbytes == 1) {
		oddbyte = 0;
		*((short int *) & oddbyte) = *(short int *) ptr;
		sum += oddbyte;
	}

	sum = (sum >> 16) + (sum & 0xffff);
	sum += (sum >> 16);
	answer = ~sum;

	return (answer);
}
