int m = 1;
004119FE  mov         dword ptr [m],1 
  if (m * m)
00411A05  mov         eax,dword ptr [m] 
00411A08  imul        eax,dword ptr [m] 
00411A0C  test        eax,eax 
00411A0E  je          main+39h (411A19h) 
  {
    m=2;
00411A10  mov         dword ptr [m],2 
  }
  else
00411A17  jmp         main+40h (411A20h) 
  {
    m = 3;
00411A19  mov         dword ptr [m],3 
  }
  
  
  
