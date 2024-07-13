/* arcfour.c */

#include "arcfour.h"

export Arcfour *rc4init(int8 *key,int16 size){
    int16 x;
    int8 temp1,temp2;
    Arcfour *p;
    int32 n;

    if(!(p = malloc(sizeof(struct s_arcfour)))){
        assert_perror(errno);
    }

    for(x=0;x<256;x++){
        p->s[x]=0;
    }   
    p->i=p->j=p->k=0;
    temp1=temp2=0;

    for(p->i=0;p->i<256;p->i++){
       p->s[p->i]=p->i;
    }

    for(p->i=0;p->i<256;p->i++){
                 temp1 = p->i% size;
                 temp2 = p->j+ p->s[p->i]+key[temp1];
                 p->j = temp2 %256;
                 temp1 = p->s[p->i];
                 temp2 = p->s[p->j];
                 p->s[p->j]=temp1;
                 p->s[p->i]=temp2;
    }
    p->i=p->j=0;
    rc4whitewash(n,p);
    return p;

}

int rc4byte(Arcfour *p){
    int16 temp1,temp2;
    
    p->i = (p->i+1)%256;
    p->j = (p->j+p->s[p->i])%256;
    temp1 = p->s[p->i];
    temp2 = p->s[p->j];
    p->s[p->j]=temp1;
    p->s[p->i]=temp2;
    temp1= (p->s[p->i]+p->s[p->j])%256;
    p->k = p->s[temp1];

    return p->k;
}

export int8 *rc4_encrypt(Arcfour *p,int8 *cleartext,int16 size){
         int8 *ciphertext;
         int16 x;

         ciphertext = (int8 *)malloc(size+1);
         if(!ciphertext){
             assert_perror(errno);
         }

         for(x=0;x<size;x++){
             ciphertext[x]=cleartext[x]^rc4byte(p);
         }
          
        return ciphertext;

}