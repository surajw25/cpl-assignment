#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int divsion_in_strings(char* a);
void multiply_Bigint(uint32_t * a, uint32_t * b, uint32_t * r);
void add_Bigint(uint32_t* a, uint32_t* b, uint32_t * res);
void subtract_Bigint(uint32_t * a, uint32_t * b, uint32_t * res);
void display(uint32_t* a);

//<------TO CREATE A BIGINT DATA-TYPE AND PERFORM ADDITION, SUBTRACTION, MULTIPILCATION OPERATIONS ON IT------------->


struct Bigint
{
    uint32_t * num;
    int pow;
};


int divsion_in_strings(char* a)
{
    //To divide a number in form of string
    //Use: To find the binary equivalent of the string input
    char* ans = (char *)malloc(sizeof(char)*350);
    int idx,temp,divisor;
    idx = 0;
    temp = a[idx] - '0';
    divisor = 2;
    int remainder=0;
    if (a[0]=='1')
    {
        remainder=1;
    }
    while (temp < divisor)
    {
        temp = temp * 10 + (a[++idx] - '0');
    }
    int i=0;
    while (strlen(a) > idx)
    {
        ans[i++] = (temp / divisor) + '0';
        remainder = temp % divisor;
        temp = (remainder) * 10 + a[++idx] - '0';
    }
    ans[i]='\0';
    strcpy(a,ans);
    free(ans);
    //Copy back ans to string a
    //This is to maintain the recurrence of the function till the all the bits are obtained
    return remainder;
}


uint32_t* initialise(char * a, struct Bigint * no)
{
    scanf("%s ",a);         //To take input of number as string
    //Generate binary equivalent of the number and enter the value bit-by-bit in the unsigned integer array
    int i,j,ct,k;
    ct=0;
    uint32_t val;
    uint32_t * numb = (uint32_t *)malloc(sizeof(uint32_t)*32);
    i=31;           //To save number in reverse
    while (i>=0 && strlen(a)!=0)
    {
        j=0;
        val=0;
        while (j<32 && strlen(a)!=0)
        {
            k = divsion_in_strings(a);
            val = (k << j) | val;       //Using bitwise shift to multiply by 2 iteratively and thus obtaining a unsigned 32 bit number
            j++;
            ct++;
        }
        numb[i--] = val;
    }
    no->pow = ct;
    while (i>=0)
    {
        numb[i--]=0;
    }
    if (strlen(a)!=0)               //Condition for overflow
    {
        printf("Overflow");
        free(numb);
        numb = NULL;
        return numb;
    }
    no->num = numb;
    return numb;
}

void multiply_Bigint(uint32_t * a, uint32_t * b, uint32_t* r)
{
    //Approach : Multiplying each bit of number1 to to each bit of number2
    uint32_t** ar = (uint32_t **)malloc(sizeof(uint32_t *)*1024);       //A temporary 2D array to store the values of bit multiplications row-wise
    int ct,carry,i,j,k,t;
    for (i=0; i<1024; i++)
    {
        ar[i] = (uint32_t *)malloc(sizeof(uint32_t)*32);
    }
    uint32_t br[32]={0};                                //Temporary array to store the state of bitwise shift
    for (i=0; i<32; i++)
    {
        br[i]=a[i];
    }
    uint32_t v;
    ct=0;
    carry=0;
    i=31;
    while (i>=0)
    {
        v = b[i];
        for (j=0; j<32; j++)
        {
            k = v%2;                    //Finding each bit of number2
            v = v/2;
            if (ct == 0)                //first row has no shift
            {
                if (k==0)               //if bit is 0: product of bit and number1 will be 0
                {
                    for (t=0; t<32; t++)
                    {
                        ar[ct][t] = 0;
                    }
                    ct++;
                }
                else                    //if bit is 1: product of bit and number1 will be number1
                {
                    for (t=0; t<32; t++)
                    {
                        ar[ct][t] = a[t];
                    }
                    ct++;
                }
            }
            else
            {
                uint64_t w;
                carry=0;
                for (t=31; t>=0; t--)                   //to create a shift
                {
                    w = (((uint64_t)br[t]) << 1) | carry;           //left shift the number and add the carry from previous block of 32 bits
                    carry = (uint32_t)((w >> 32));                  //To obtain carry for next block : the first bit of the original block
                    br[t] = (uint32_t)w;
                }
                if (k == 0)
                {
                    for (t=0; t<32; t++)
                    {
                        ar[ct][t] = 0;
                    }
                    ct++;
                }
                else
                {
                    for (t=0; t<32; t++)
                    {
                        ar[ct][t] = br[t];
                    }
                    ct++;
                }
            }
        }
        i--;
    }
    for (i=0; i<32; i++)
    {
        r[i]=0;
    }
    i=0;
    while (i<1024 && r!=NULL)
    {
        add_Bigint(r,ar[i],r);
        i++;
    }
    for (i=0; i<32; i++)
    {
        free(ar[i]);
    }
    free(ar);
}

void add_Bigint(uint32_t* a, uint32_t* b, uint32_t * res)
{
    //Approach : Similar to Full Adder
    int i,j,carry,sum;
    uint32_t val=0;
    uint32_t m;
    uint32_t n;
    int a1,b1;
    carry=0;
    for (i=31; i>=0; i--)
    {
        m = a[i];
        n = b[i];
        sum = 0;
        val = 0;
        for (j=0; j<32; j++)
        {
            a1 = m%2;
            b1 = n%2;
            sum = ((a1) ^ (b1)) ^ carry;
            val = (sum << j) | val;
            carry = (((a1)&(b1)) | ((b1)&carry) | ((carry)&(a1)));
            m = m/2;
            n = n/2;
        }
        res[i] = val;
    }
    if (carry == 1)
    {
        free(res);
        res = NULL;
    }
}

void subtract_Bigint(uint32_t * a, uint32_t * b, uint32_t * res)
{
    int i,j,carry,sum;
    uint32_t val=0;
    uint32_t m;
    uint32_t n;
    int t,comp,a1,b1;
    comp=1;
    carry=0;
    for (i=31; i>=0; i--)
    {
        m = a[i];
        n = b[i];
        sum = 0;
        val = 0;
        for (j=0; j<32; j++)
        {
            a1 = m%2;
            t = (1 ^ (n%2));
            b1 = t ^ comp;
            comp = comp & t;
            sum = ((a1) ^ (b1)) ^ carry;
            val = (sum << j) | val;
            carry = (((a1)&(b1)) | ((b1)&carry) | ((carry)&(a1)));
            m = m/2;
            n = n/2;
        }
        res[i] = val;
    }
}

void display(uint32_t* a)
{
    uint32_t v;
    int i,j,k,n,carry;
    int y[32];
    uint32_t br[32];
    int q = 2000;
    char p[q];
    if (a==NULL)
    {
        printf("Overflow");
    }
    else
    {
        for (i=0; i<q; i++)
        {
            p[i]='0';
        }
        p[q-1]='\0';
        for (i=0; i<32; i++)
        {
            v = a[i];
            for (j=0; j<32; j++)
            {
                y[31-j] = v%2;
                v = v/2;
            }
            for (j=0; j<32; j++)
            {
                n=0;
                carry = 0;
                for (k=q-2; k>=0; k--)
                {
                    n =(((int)p[k] - 48)*(2)) + carry;
                    carry = n/10;
                    p[k] = (char)(n%10 + 48);
                }
                carry = y[j];
                for (k=q-2; k>=0; k--)
                {
                    n = (int)p[k] + carry - 48;
                    carry = n/10;
                    p[k] = (char)(n%10 + 48);
                }
            }
        }
        i=0;
        while (i<q-2 && p[i]=='0')
        {
            i++;
        }
        int start;
        start = 0;
        for (; i<q-1; i++)
        {
            p[start++] = p[i];
        }
        p[start]='\0';
        printf("%s ",p);
    }
}

int main()
{
    struct Bigint n1;
    struct Bigint n2;
    //Input to the number taken as string
    char * a = (char *)malloc(sizeof(char)*350);
    uint32_t* a1 = initialise(a,&n1);

    char * b = (char *)malloc(sizeof(char)*350);
    uint32_t* b1 = initialise(b,&n2);

    if (a1!=NULL && b1!=NULL)
    {
        //ADDITION
        uint32_t* res1 = (uint32_t *)malloc(sizeof(uint32_t)*32);
        add_Bigint(n1.num,n2.num,res1);
        printf("Addition is: ");
        display(res1);
        printf("\n");

        //SUBTRACTION
        uint32_t* res2 = (uint32_t *)malloc(sizeof(uint32_t)*32);
        subtract_Bigint(n1.num,n2.num,res2);
        printf("Subtraction is: ");
        display(res2);
        printf("\n");

        //MULTIPLICATION
        uint32_t* res3 = (uint32_t *)malloc(sizeof(uint32_t)*32);
        if (n1.pow+n2.pow>1024)
        {
            printf("Overflow");
        }
        else
        {
            multiply_Bigint(n1.num,n2.num,res3);
            printf("Product is: ");
            display(res3);
        }
    }
    else
    {
        printf("OVERFLOW IN EITHER OF THE INPUTS. PLEASE TRY AGAIN.......");
    }
    return 0;
}
