
#include        <string.h> 
#include        <ctype.h> 

char *soundex(const char *in)
{
   static  int code[] =
      {  0,1,2,3,0,1,2,0,0,2,2,4,5,5,0,1,2,6,2,3,0,1,0,2,0,2 };
      /* a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z */
   static  char key[5];
   register        char ch;
   register        int last;
   register        int count;

   /* Set up default key, complete with trailing '0's */
   strcpy(key, "Z000");

   /* Advance to the first letter.  If none present, 
      return default key */
   while (*in != '\0'  &&  !isalpha(*in))
      ++in;
   if (*in == '\0')
      return key;

   /* Pull out the first letter, uppercase it, and 
      set up for main loop */
   key[0] = toupper(*in);
   last = code[key[0] - 'A'];
   ++in;

   /* Scan rest of string, stop at end of string or 
      when the key is full */
   for (count = 1;  count < 4  &&  *in != '\0';  ++in) {
      /* If non-alpha, ignore the character altogether */
      if (isalpha(*in)) {
         ch = tolower(*in);
         /* Fold together adjacent letters sharing the same code */
         if (last != code[ch - 'a']) {
            last = code[ch - 'a'];
            /* Ignore code==0 letters except as separators */
            if (last != 0)
               key[count++] = '0' + last;
         }
      }
   }

   return key;
}
/*int main()
{
   char    instring[80];

   while (fgets(instring, sizeof instring, stdin) != NULL)
      printf("%s\n", soundex(instring));

   return 0;
}*/
