#include <string.h>

#ifndef LITTLE_ENDIAN 
#define	LITTLE_ENDIAN
#endif	

typedef struct
{
	unsigned int  k_len;
	unsigned int  e_key[64];
	unsigned int  d_key[64];
}STR_AES;

#ifdef _MSC_VER

#  include <stdlib.h>
#  pragma intrinsic(_lrotr,_lrotl)
#  define rotr(x,n) _lrotr(x,n)
#  define rotl(x,n) _lrotl(x,n)

#else

#define rotr(x,n)   (((x) >> ((int)(n))) | ((x) << (32 - (int)(n))))
#define rotl(x,n)   (((x) << ((int)(n))) | ((x) >> (32 - (int)(n))))

#endif

/* Invert byte order in a 32 bit variable                           */

#define bswap(x)    (rotl(x, 8) & 0x00ff00ff | rotr(x, 8) & 0xff00ff00)

/* Extract byte from a 32 bit quantity (little endian notation)     */ 

#define aes_byte(x,n)   ((unsigned char)((x) >> (8 * n)))

/* Input or output a 32 bit word in machine order					*/

#ifdef	LITTLE_ENDIAN

#define	un_long_in(x)		(*(unsigned int*)(x))
#define	un_long_out(x, v)	(*(unsigned int*)(x) = (v)) 

#else

#define	un_long_in(x)		bswap(*(unsigned int)(x))
#define	un_long_out(x, v)	(*(unsigned int*)(x) = bswap(v)) 

#endif


#define LARGE_TABLES

unsigned char  pow_tab[256];
unsigned char  log_tab[256];
unsigned char  sbx_tab[256];
unsigned char  isb_tab[256];
unsigned int  rco_tab[ 10];
unsigned int  ft_tab[4][256];
unsigned int  it_tab[4][256];

#ifdef  LARGE_TABLES
  unsigned int  fl_tab[4][256];
  unsigned int  il_tab[4][256];
#endif

unsigned int  tab_gen = 0;

#define ff_mult(a,b)    (a && b ? pow_tab[(log_tab[a] + log_tab[b]) % 255] : 0)

#define f_rn(bo, bi, n, k)                          \
    bo[n] =  ft_tab[0][aes_byte(bi[n],0)] ^             \
             ft_tab[1][aes_byte(bi[(n + 1) & 3],1)] ^   \
             ft_tab[2][aes_byte(bi[(n + 2) & 3],2)] ^   \
             ft_tab[3][aes_byte(bi[(n + 3) & 3],3)] ^ *(k + n)

#define i_rn(bo, bi, n, k)                          \
    bo[n] =  it_tab[0][aes_byte(bi[n],0)] ^             \
             it_tab[1][aes_byte(bi[(n + 3) & 3],1)] ^   \
             it_tab[2][aes_byte(bi[(n + 2) & 3],2)] ^   \
             it_tab[3][aes_byte(bi[(n + 1) & 3],3)] ^ *(k + n)

#ifdef LARGE_TABLES

#define ls_box(x)                \
    ( fl_tab[0][aes_byte(x, 0)] ^    \
      fl_tab[1][aes_byte(x, 1)] ^    \
      fl_tab[2][aes_byte(x, 2)] ^    \
      fl_tab[3][aes_byte(x, 3)] )

#define f_rl(bo, bi, n, k)                          \
    bo[n] =  fl_tab[0][aes_byte(bi[n],0)] ^             \
             fl_tab[1][aes_byte(bi[(n + 1) & 3],1)] ^   \
             fl_tab[2][aes_byte(bi[(n + 2) & 3],2)] ^   \
             fl_tab[3][aes_byte(bi[(n + 3) & 3],3)] ^ *(k + n)

#define i_rl(bo, bi, n, k)                          \
    bo[n] =  il_tab[0][aes_byte(bi[n],0)] ^             \
             il_tab[1][aes_byte(bi[(n + 3) & 3],1)] ^   \
             il_tab[2][aes_byte(bi[(n + 2) & 3],2)] ^   \
             il_tab[3][aes_byte(bi[(n + 1) & 3],3)] ^ *(k + n)

#else

#define ls_box(x)                            \
    ((unsigned int)sbx_tab[aes_byte(x, 0)] <<  0) ^    \
    ((unsigned int)sbx_tab[aes_byte(x, 1)] <<  8) ^    \
    ((unsigned int)sbx_tab[aes_byte(x, 2)] << 16) ^    \
    ((unsigned int)sbx_tab[aes_byte(x, 3)] << 24)

#define f_rl(bo, bi, n, k)                                      \
    bo[n] = (unsigned int)sbx_tab[aes_byte(bi[n],0)] ^                    \
        rotl(((unsigned int)sbx_tab[aes_byte(bi[(n + 1) & 3],1)]),  8) ^  \
        rotl(((unsigned int)sbx_tab[aes_byte(bi[(n + 2) & 3],2)]), 16) ^  \
        rotl(((unsigned int)sbx_tab[aes_byte(bi[(n + 3) & 3],3)]), 24) ^ *(k + n)

#define i_rl(bo, bi, n, k)                                      \
    bo[n] = (unsigned int)isb_tab[aes_byte(bi[n],0)] ^                    \
        rotl(((unsigned int)isb_tab[aes_byte(bi[(n + 3) & 3],1)]),  8) ^  \
        rotl(((unsigned int)isb_tab[aes_byte(bi[(n + 2) & 3],2)]), 16) ^  \
        rotl(((unsigned int)isb_tab[aes_byte(bi[(n + 1) & 3],3)]), 24) ^ *(k + n)

#endif

void gen_tabs(void)
{   unsigned int  i, t;
    unsigned char  p, q;

    // log and power tables for GF(2**8) finite field with  
    // 0x011b as modular polynomial - the simplest prmitive 
    // root is 0x03, used here to generate the tables       

    for(i = 0,p = 1; i < 256; ++i)
    {
        pow_tab[i] = (unsigned char)p; log_tab[p] = (unsigned char)i;

        p = p ^ (p << 1) ^ (p & 0x80 ? 0x01b : 0);
    }

    log_tab[1] = 0; p = 1;

    for(i = 0; i < 10; ++i)
    {
        rco_tab[i] = p; 

        p = (p << 1) ^ (p & 0x80 ? 0x1b : 0);
    }

    for(i = 0; i < 256; ++i)
    {   
        p = (i ? pow_tab[255 - log_tab[i]] : 0); q = p; 
        q = (q >> 7) | (q << 1); p ^= q; 
        q = (q >> 7) | (q << 1); p ^= q; 
        q = (q >> 7) | (q << 1); p ^= q; 
        q = (q >> 7) | (q << 1); p ^= q ^ 0x63; 
        sbx_tab[i] = p; isb_tab[p] = (unsigned char)i;
    }

    for(i = 0; i < 256; ++i)
    {
        p = sbx_tab[i]; 

#ifdef  LARGE_TABLES        
        
        t = p; fl_tab[0][i] = t;
        fl_tab[1][i] = rotl(t,  8);
        fl_tab[2][i] = rotl(t, 16);
        fl_tab[3][i] = rotl(t, 24);
#endif
        t = ((unsigned int)ff_mult(2, p)) |
            ((unsigned int)p <<  8) |
            ((unsigned int)p << 16) |
            ((unsigned int)ff_mult(3, p) << 24);
        
        ft_tab[0][i] = t;
        ft_tab[1][i] = rotl(t,  8);
        ft_tab[2][i] = rotl(t, 16);
        ft_tab[3][i] = rotl(t, 24);

        p = isb_tab[i]; 

#ifdef  LARGE_TABLES        
        
        t = p; il_tab[0][i] = t; 
        il_tab[1][i] = rotl(t,  8); 
        il_tab[2][i] = rotl(t, 16); 
        il_tab[3][i] = rotl(t, 24);
#endif 
        t = ((unsigned int)ff_mult(14, p)) |
            ((unsigned int)ff_mult( 9, p) <<  8) |
            ((unsigned int)ff_mult(13, p) << 16) |
            ((unsigned int)ff_mult(11, p) << 24);
        
        it_tab[0][i] = t; 
        it_tab[1][i] = rotl(t,  8); 
        it_tab[2][i] = rotl(t, 16); 
        it_tab[3][i] = rotl(t, 24); 
    }

    tab_gen = 1;
}

#define star_x(x) (((x) & 0x7f7f7f7f) << 1) ^ ((((x) & 0x80808080) >> 7) * 0x1b)

#define imix_col(y,x)       \
    u   = star_x(x);        \
    v   = star_x(u);        \
    w   = star_x(v);        \
    t   = w ^ (x);          \
   (y)  = u ^ v ^ w;        \
   (y) ^= rotr(u ^ t,  8) ^ \
          rotr(v ^ t, 16) ^ \
          rotr(t,24)


// initialise the key schedule from the user supplied key   

#define loop4(i)                                    \
{   t = ls_box(rotr(t,  8)) ^ rco_tab[i];           \
    t ^= e_key[4 * i];     e_key[4 * i + 4] = t;    \
    t ^= e_key[4 * i + 1]; e_key[4 * i + 5] = t;    \
    t ^= e_key[4 * i + 2]; e_key[4 * i + 6] = t;    \
    t ^= e_key[4 * i + 3]; e_key[4 * i + 7] = t;    \
}
#define loop6(i)                                    \
{   t = ls_box(rotr(t,  8)) ^ rco_tab[i];           \
    t ^= e_key[6 * i];     e_key[6 * i + 6] = t;    \
    t ^= e_key[6 * i + 1]; e_key[6 * i + 7] = t;    \
    t ^= e_key[6 * i + 2]; e_key[6 * i + 8] = t;    \
    t ^= e_key[6 * i + 3]; e_key[6 * i + 9] = t;    \
    t ^= e_key[6 * i + 4]; e_key[6 * i + 10] = t;   \
    t ^= e_key[6 * i + 5]; e_key[6 * i + 11] = t;   \
}

#define loop8(i)                                    \
{   t = ls_box(rotr(t,  8)) ^ rco_tab[i];           \
    t ^= e_key[8 * i];     e_key[8 * i + 8] = t;    \
    t ^= e_key[8 * i + 1]; e_key[8 * i + 9] = t;    \
    t ^= e_key[8 * i + 2]; e_key[8 * i + 10] = t;   \
    t ^= e_key[8 * i + 3]; e_key[8 * i + 11] = t;   \
    t  = e_key[8 * i + 4] ^ ls_box(t);              \
    e_key[8 * i + 12] = t;                          \
    t ^= e_key[8 * i + 5]; e_key[8 * i + 13] = t;   \
    t ^= e_key[8 * i + 6]; e_key[8 * i + 14] = t;   \
    t ^= e_key[8 * i + 7]; e_key[8 * i + 15] = t;   \
}

void set_key(STR_AES * pStrAES, const unsigned char in_key[], const unsigned int key_len)
{   
	unsigned int  i, t, u, v, w;
	unsigned int  *e_key, *d_key;
	unsigned int  k_len;
	e_key = pStrAES->e_key;
	d_key = pStrAES->d_key;
	
	
    if(!tab_gen)
        gen_tabs();

    k_len = (key_len + 31) / 32;
pStrAES->k_len = k_len;
	
    e_key[0] = un_long_in(in_key     ); 
	e_key[1] = un_long_in(in_key +  4);
    e_key[2] = un_long_in(in_key +  8); 
	e_key[3] = un_long_in(in_key + 12);

    switch(k_len)
    {
        case 4: t = e_key[3];
                for(i = 0; i < 10; ++i) 
                    loop4(i);
                break;

        case 6: e_key[4] = un_long_in(in_key + 16); t = e_key[5] = un_long_in(in_key + 20);
                for(i = 0; i < 8; ++i) 
                    loop6(i);
                break;

        case 8: e_key[4] = un_long_in(in_key + 16); e_key[5] = un_long_in(in_key + 20);
                e_key[6] = un_long_in(in_key + 24); t = e_key[7] = un_long_in(in_key + 28);
                for(i = 0; i < 7; ++i) 
                    loop8(i);
                break;
    }

    d_key[0] = e_key[0]; d_key[1] = e_key[1];
    d_key[2] = e_key[2]; d_key[3] = e_key[3];

    for(i = 4; i < 4 * k_len + 24; ++i)
    {
        imix_col(d_key[i], e_key[i]);
    }

    return;
}

// encrypt a block of text  

#define f_nround(bo, bi, k) \
    f_rn(bo, bi, 0, k);     \
    f_rn(bo, bi, 1, k);     \
    f_rn(bo, bi, 2, k);     \
    f_rn(bo, bi, 3, k);     \
    k += 4

#define f_lround(bo, bi, k) \
    f_rl(bo, bi, 0, k);     \
    f_rl(bo, bi, 1, k);     \
    f_rl(bo, bi, 2, k);     \
    f_rl(bo, bi, 3, k)

int expand_key(unsigned char* ucSrcPwd,int _iSrcLen, unsigned char* ucDstPwd)
{
    int i;

    if(_iSrcLen>16)
        return -1;

    memcpy(ucDstPwd,ucSrcPwd,_iSrcLen);
	
    for(i=_iSrcLen;i<16;i++)
    {
        ucDstPwd[i]=0xff;
    }
	
    ucDstPwd[i]=0;

    return 0;
}

void aes_encrypt(STR_AES * pStrAES,const unsigned char in_blk[16], unsigned char out_blk[16])
{   
	unsigned int  b0[4], b1[4], *kp;
	unsigned int  *e_key, *d_key;
	unsigned int  k_len;

	e_key = pStrAES->e_key;
	d_key = pStrAES->d_key;
	k_len = pStrAES->k_len;
	
    b0[0] = un_long_in(in_blk     ) ^ e_key[0]; 
	b0[1] = un_long_in(in_blk +  4) ^ e_key[1];
    b0[2] = un_long_in(in_blk +  8) ^ e_key[2]; 
	b0[3] = un_long_in(in_blk + 12) ^ e_key[3];

    kp = e_key + 4;

    if(k_len > 6)
    {
        f_nround(b1, b0, kp); f_nround(b0, b1, kp);
    }

    if(k_len > 4)
    {
        f_nround(b1, b0, kp); f_nround(b0, b1, kp);
    }

    f_nround(b1, b0, kp); f_nround(b0, b1, kp);
    f_nround(b1, b0, kp); f_nround(b0, b1, kp);
    f_nround(b1, b0, kp); f_nround(b0, b1, kp);
    f_nround(b1, b0, kp); f_nround(b0, b1, kp);
    f_nround(b1, b0, kp); f_lround(b0, b1, kp);

    un_long_out(out_blk,      b0[0]); un_long_out(out_blk +  4, b0[1]);
    un_long_out(out_blk +  8, b0[2]); un_long_out(out_blk + 12, b0[3]);
}

// decrypt a block of text  

#define i_nround(bo, bi, k) \
    i_rn(bo, bi, 0, k);     \
    i_rn(bo, bi, 1, k);     \
    i_rn(bo, bi, 2, k);     \
    i_rn(bo, bi, 3, k);     \
    k -= 4

#define i_lround(bo, bi, k) \
    i_rl(bo, bi, 0, k);     \
    i_rl(bo, bi, 1, k);     \
    i_rl(bo, bi, 2, k);     \
    i_rl(bo, bi, 3, k)

void aes_decrypt(STR_AES * pStrAES,const unsigned char in_blk[16], unsigned char out_blk[16])
{   
	unsigned int  b0[4], b1[4], *kp;
	unsigned int  *e_key, *d_key;
	unsigned int  k_len;

	e_key = pStrAES->e_key;
	d_key = pStrAES->d_key;
	k_len = pStrAES->k_len;

    b0[0] = un_long_in(in_blk     ) ^ e_key[4 * k_len + 24]; 
	b0[1] = un_long_in(in_blk +  4) ^ e_key[4 * k_len + 25];
    b0[2] = un_long_in(in_blk +  8) ^ e_key[4 * k_len + 26]; 
	b0[3] = un_long_in(in_blk + 12) ^ e_key[4 * k_len + 27];

    kp = d_key + 4 * (k_len + 5);

    if(k_len > 6)
    {
        i_nround(b1, b0, kp); i_nround(b0, b1, kp);
    }

    if(k_len > 4)
    {
        i_nround(b1, b0, kp); i_nround(b0, b1, kp);
    }

    i_nround(b1, b0, kp); i_nround(b0, b1, kp);
    i_nround(b1, b0, kp); i_nround(b0, b1, kp);
    i_nround(b1, b0, kp); i_nround(b0, b1, kp);
    i_nround(b1, b0, kp); i_nround(b0, b1, kp);
    i_nround(b1, b0, kp); i_lround(b0, b1, kp);

    un_long_out(out_blk,     b0[0]); un_long_out(out_blk +  4, b0[1]);
    un_long_out(out_blk + 8, b0[2]); un_long_out(out_blk + 12, b0[3]);
}

int aes_crypt(unsigned char* _pcVData,int _iDataLen,unsigned int _iCTRKey,unsigned char* _pucAESKey,int _iAESKeyLen)
{
    //首先对密码进行扩展
    STR_AES strAesEnc;
    unsigned char ucExpandKey[17];
    unsigned char ucCrtKey[17];
    unsigned char ucTmp[16];
    int i;

    memset(ucExpandKey,0,17);
    expand_key(_pucAESKey,_iAESKeyLen,ucExpandKey);

    memset(ucCrtKey,0,17);
    memcpy(ucCrtKey,&_iCTRKey,sizeof(int));

    memset(&strAesEnc,0,sizeof(strAesEnc));
    set_key(&strAesEnc,ucExpandKey,128);
    aes_encrypt(&strAesEnc,ucCrtKey,ucTmp);

    for(i=0;i<_iDataLen;i++)
        _pcVData[i]^=ucTmp[i%16];

    return 0;
}

