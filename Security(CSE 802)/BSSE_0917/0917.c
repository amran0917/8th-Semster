#include <stdio.h>
#include <string.h>
#include <openssl/des.h>
#include <stdlib.h>
#include <math.h>


#define ENC 1
#define DEC 0

int HexChartoIntiger(unsigned char ch){
    int num=0;
    if(ch>='0' && ch<='9')
    {
        num=ch-0x30;
    } 

	else if ( ch=='A' || ch == 'a') num = 10;
	else if ( ch=='B' || ch == 'b') num = 11;
	else if ( ch=='C' || ch == 'c') num = 12;
	else if ( ch=='D' || ch == 'd') num = 13;
	else if ( ch=='E' || ch == 'e') num = 14;
	else if ( ch=='F' || ch == 'f') num = 15;
else{
	num=0;
}
   
    return num;
}

unsigned int convert(int bit32[32]){
    unsigned int ans=0;
    printf("\n");

    for(int i=0;i<32;i++){
        ans+= (bit32[i]*pow(2,i));
    }
    
    return ans;
}


unsigned int * getXORValue(unsigned char *paintext, unsigned char *IV){

    int bitArray[8];
    int allBitsIV[64];
    int allBitsPaintext[64];
    int allCharofIV[16];
    int *xorValue = malloc(sizeof(int)*64);

    printf("\n");
    for(int i=0;i<8;i++){
         bitArray[i]=0;
    }
    int jump=0;
    for(int j=0;j<16;j++){
        for(int i=7;i>=0;i--){
            bitArray[7-i]= ((HexChartoIntiger(IV[j])>>i)&1 ? 1 : 0);
        }
        int incrementedValue = 4;
        for(int i=0;i<4;i++){
            allBitsIV[j*incrementedValue+i] = bitArray[incrementedValue+i];
        }
    }

    //for(int i=0;i<64;i++)printf("%d",allBitsIV[i]);
    printf("\n");

    for(int i=0;i<8;i++){
         bitArray[i]=0;
    }
    jump=0;
    for(int j=0;j<8;j++){
        for(int i=7;i>=0;i--){
            bitArray[7-i]= (((int)(paintext[j])>>i)&1 ? 1 : 0);
           //printf("%d",(int)paintext[j]);
        }

        for(int i=0;i<8;i++){
            allBitsPaintext[j*8+i] = bitArray[i];
        }
    }
    printf("\n");
    for(int i=0;i<64;i++){
        xorValue[i] = allBitsPaintext[i]^allBitsIV[i];
    }
    printf("\n");
    // for(int i=0;i<64;i++)printf("%d",xorValue[i]);
    printf("\n");

    unsigned int *unsignedXOR = malloc(sizeof(unsigned int)*2);

    int bit32[32];

    for(int i=0;i<64;i++){
        //printf("%d",xorBits[i]);
        if(i<32){
            bit32[i] = xorValue[i];
            if(i==31){
                unsignedXOR[0] = convert(bit32);
            }
        }
        else{
            bit32[i-32] = xorValue[i];
            if(i==63){
                unsignedXOR[1] = convert(bit32);
            }
        }  
    }

    return unsignedXOR;
}


int * getBinaryX(unsigned int *X){
    unsigned int a = X[0];
    unsigned int b = X[1];

   int *binaryX = malloc(sizeof(int)*64);
   int temp[64];
    int i=0;
   while(a>0){
       temp[i] = a%2;
       a=a/2;
       i++;
   }
   while(b>0){
       temp[i] = b%2;
       b=b/2;
       i++;
   }
   for(int j=0;j<i;j++){
       binaryX[j] = temp[i-j-1];
   }

   return binaryX;
}

int * getBinaryPlain(unsigned char *plaintext){
   
    printf("\n");
    int *binaryPlaintext = malloc(sizeof(int)*64);
    int bitArray[8];

    for(int j=0;j<8;j++){
        for(int i=7;i>=0;i--){
            bitArray[7-i]= (((int)(plaintext[j])>>i)&1 ? 1 : 0);
           //printf("%d",(int)paintext[j]);
        }
      
        for(int i=0;i<8;i++){
            binaryPlaintext[j*8+i] = bitArray[i];
        }
    }

    return binaryPlaintext;
}


int main(int argc, char *argv[])
{
	int k;
	//const_DES_cblock cbc_key[8] = {0x10,0x23,0x45,0x67,0x89,0xab,0xcd,0xef};
	DES_key_schedule key;

    unsigned char *IV = argv[1];
    //const_DES_cblock cbc_key;
    //memset(cbc_key,0,sizeof(cbc_key));
    unsigned char *usekey = argv[2]; 
    const_DES_cblock cbc_key;
    // //unsigned int cbc_key[8];
    int byte;
    for (int i=0;i<8;i++){
        if(sscanf(usekey,"%2X",&byte)!=1){
            break;
        }
        cbc_key[i] = byte;
        printf("%d",byte);
        usekey+=2;
    }



    FILE *fp;
    fp=fopen(argv[3],"r");
    char ch;
    char chArray[1000];
    if(fp==NULL){
        printf("Cannot open file");
    }
    int index=0;
    while((ch=fgetc(fp))!=EOF){
        printf("%c",ch);
        chArray[index] = ch;
        index++;
    }
    printf("\n");
    printf("%d",index);
    printf("\n");
    fclose(fp);
 
	if ((k = DES_set_key_checked(cbc_key,&key)) != 0)
		// printf("\nkey error\n");
    int * xorBits;
    
    
    //plainText 2d Array
    int totalPlainBlock;
    if(index%8>0){
        totalPlainBlock = (index/8) +1;
        for(int i=index;i<totalPlainBlock*8;i++)
            chArray[i]='0';
    }
    else{
        totalPlainBlock = (index/8);
    }


    unsigned char plainTextArray[totalPlainBlock][8];
    for (int i = 0; i < totalPlainBlock*8; i++)
    {
        plainTextArray[i/8][i%8] = chArray[i];
    }
    printf("\n");
    for(int i=0;i<totalPlainBlock;i++){
        for(int j=0;j<8;j++){
            printf("%c",plainTextArray[i][j]);
        }
        printf("\n");
    }

    //unsigned char IV[16] = "0123456789abcdef";
    //unsigned char paintext[8] = plainTextArray[0];
    //printf("%s",argv[1]);
    unsigned int *X;

    X = getXORValue((unsigned char*)plainTextArray[0],(unsigned char*)IV);
    
	printf("DES Clear Text: %u%u\n",X[0],X[1]);
	DES_encrypt1(X,&key,ENC);

	printf("DES Encryption: %u%u\n",X[0],X[1]);
    
    FILE *fileOut;
    fileOut = fopen(argv[4],"wb");

    FILE *fp1;
    char output[100];
    
    fp1 = fopen( argv[4] , "w" );
    sprintf(output,"%u",X[0]);
    fwrite(output , 1 , strlen(output) , fp1 );
    fwrite(" " , 1 , 1 , fp1 );
    sprintf(output,"%u",X[1]);
    fwrite(output , 1 , strlen(output) , fp1 );
    fwrite(X , 8 , 1 , fileOut);
    fwrite("\n" , 1 , 1 , fp1 );

    DES_encrypt1(X,&key,DEC);
    printf("DES Encryption: %u%u\n",X[0],X[1]);

    for(int iteration=1;iteration<totalPlainBlock;iteration++){
        unsigned int * encryptedBinary;
        encryptedBinary = getBinaryX(X);
        

        unsigned int *plaintextBinary;
        plaintextBinary = getBinaryPlain(plainTextArray[iteration]);


        int XOR[64];
        int tempXORvalue[32];

        for(int i=0;i<64;i++){
            XOR[i] = encryptedBinary[i]^plaintextBinary[i];
        }

        for(int i=0;i<64;i++){
            //printf("%d",XOR[i]);
            if(i<32){
                tempXORvalue[i] = XOR[i];
                if(i==31){
                    X[0] = convert(tempXORvalue);
                }
            }
            else{
                tempXORvalue[i-32] = XOR[i];
                if(i==63){
                    X[1] = convert(tempXORvalue);
                }
            }  
        }

        printf("DES Clear Text: %u%u\n",X[0],X[1]);
        DES_encrypt1(X,&key,ENC);

        printf("DES Encryption: %u%u\n",X[0],X[1]);
        sprintf(output,"%u",X[0]);
        fwrite(output , 1 , strlen(output) , fp1 );
        fwrite(" " , 1 , 1 , fp1 );
        sprintf(output,"%u",X[1]);
        fwrite(output , 1 , strlen(output) , fp1 );
        fwrite(X , 8 , 1 , fileOut);
        fwrite("\n" , 1 , 1 , fp1 );

        DES_encrypt1(X,&key,DEC);
        printf("DES Encryption: %u%u\n",X[0],X[1]);
    }
    fclose(fp1);

    //description 
	
    unsigned int temp;
    unsigned int Y[2];
    int count=0;
    char encrytstring[100];
    FILE *fp3;
    char c;
    int YRow=0;
    fp3=fopen(argv[4],"r");
    while((c = fgetc(fp3)) != EOF){ 
		if(c == ' '){ 
            count=0;
            Y[YRow] = atoi(encrytstring);
            YRow=1;
            
			printf("\n");
		}
        else if( c == '\n'){
            printf("\n");
            Y[YRow] = atoi(encrytstring);
            count=0;
            YRow=0;
            DES_encrypt1(Y,&key,DEC);
            printf("DES Encryption: %u%u\n",Y[0],Y[1]);
        }
		else { 
	
            encrytstring[count]=c;
            count++;

		} 
	}
    fclose(fp3);

}
