#include <iostream>
#include <fstream>
#include "stdlib.h"
#include <iomanip>
#include <bitset>

using namespace std;

unsigned char buffer[8 * 1024];	// Buffer to hold the machine code

void
emitError (char *s)
{
  cout << s;
  exit (0);
}



void
instDecExec (unsigned int instWord)
{
  unsigned int rd, rs1, rs2, funct3, funct7, opcode;
  int I_imm, S_imm, B_imm, U_imm, J_imm;
  unsigned int address;

  opcode = instWord & 0x0000007F;
  rd = (instWord >> 7) & 0x0000001F;
  funct3 = (instWord >> 12) & 0x00000007;
  rs1 = (instWord >> 15) & 0x0000001F;
  rs2 = (instWord >> 20) & 0x0000001F;


  if (opcode == 0x33)
    {				// R Instructions
      funct7 = (instWord >> 25);
      switch (funct3)
	{
	case 0:
	  if (funct7 == 32)
	    {
		cout << "\tSUB\tx" << rd << ", x" << rs1 << ", x" << rs2 <<
		  "\n";
	    }
	  else
	    cout << "\tADD\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
	  //regs[rd] = regs[rs1] + regs[rs2];
	  break;
	case 1:
	  cout << "\tSLL\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
	  break;
	case 2:
	  cout << "\tSLT\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
	  break;
	case 3:
	    cout << "\tSLTU\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
	  break;
	case 4:
	  cout << "\tXOR\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
	  break;
	case 5:
	  if (funct7 == 32)
	    cout << "\tSRA\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
	  else
	    cout << "\tSRL\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
	  break;
	case 6:
	  cout << "\tOR\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
	  break;
	case 7:
	  cout << "\tAND\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
	  break;

	default:
	  cout << "\tUnkown R Instruction \n";

	}
    }

  else if (opcode == 0x3)
    {
      funct7 = (instWord >> 20) & 0xfe0;
      I_imm = funct7 | rs2;

      int sign_bit = I_imm >> 11;
      if (sign_bit == 1)
	{
	  I_imm = I_imm - 4095 - 1;
	}

      switch (funct3)
	{
	case 0:
	  cout << "\tLB\tx" << rd << ", " << I_imm << "(x" << rs1 << ")\n";
	  break;
	case 1:
	  cout << "\tLH\tx" << rd << ", " << I_imm << "(x" << rs1 << ")\n";
	  break;
	case 2:
	  cout << "\tLW\tx" << rd << ", " << I_imm << "(x" << rs1 << ")\n";
	  break;
	case 4:
	  cout << "\tLBU\tx" << rd << ", " << I_imm << "(x" << rs1 << ")\n";
	  break;
	case 5:
	  cout << "\tLHU\tx" << rd << ", " << I_imm << "(x" << rs1 << ")\n";
	  break;
	}
    }
  else if (opcode == 0x13)
    {
      funct7 = (instWord >> 20) & 0xFE0;
      I_imm = funct7 | rs2;
      int sign_bit = I_imm >> 11;
      if (sign_bit == 1)
	{
	  I_imm = I_imm - 4095 - 1;
	}
      switch (funct3)
	{
	case 0:
	  {
	      {
		cout << "\tADDI\tx" << rd << ", x" << rs1 << ", " << I_imm <<
		  "\n";
	      }
	    break;
	  }
	case 2:
	  {
	    cout << "\tSLTI\tx" << rd << ", x" << rs1 << ", " << I_imm <<
	      "\n";
	    break;
	  }
	case 3:
	  {
	      {
		cout << "\tSLTIU\tx" << rd << ", x" << rs1 << ", " << I_imm <<
		  "\n";
	      }
	    break;
	  }
	case 4:
	  {
	      {
		cout << "\tXORI\tx" << rd << ", x" << rs1 << ", " << I_imm <<
		  "\n";
	      }
	    break;
	  }
	case 6:
	  {
	    cout << "\tORI\tx" << rd << ", x" << rs1 << ", " << I_imm << "\n";
	    break;
	  }
	case 7:
	  {
	    cout << "\tANDI\tx" << rd << ", x" << rs1 << ", " << I_imm <<
	      "\n";
	    break;
	  }
	case 1:
	  {
	    cout << "\tSLLI\tx" << rd << ", x" << rs1 << ", " << I_imm <<
	      "\n";
	    break;
	  }
	case 5:
	  {
	    if (funct7 == 0)
	      {
		cout << "\tSRLI\tx" << rd << ", x" << rs1 << ", " << rs2 <<
		  "\n";
	      }
	    else
	      cout << "\tSRAI\tx" << rd << ", x" << rs1 << ", " << rs2 <<
		"\n";

	    break;
	  }
	}
    }
  else if (opcode == 0x67)
    {
      I_imm = instWord >> 20;

      int sign_bit = I_imm >> 11;
      if (sign_bit == 1)
	{
	  I_imm = I_imm - 4095 - 1;
	}
	{
	  cout << "\tJALR\tx" << rd << ", x" << rs1 << ", " << I_imm << "\n";
	}
    }
  else if (opcode == 0x23)
    {

      funct7 = (instWord >> 20) & 0xFE0;
      S_imm = funct7 | rd;

      int sign_bit = S_imm >> 11;
      if (sign_bit == 1)
	{
	  S_imm = S_imm - 4095 - 1;
	}

      switch (funct3)
	{
	case 0:
	  {
	    cout << "\tSB\tx" << rd << ", " << S_imm << "(x" << rs1 << ")\n";
	    break;
	  }
	case 1:
	  {
	    cout << "\tSH\tx" << rd << ", " << S_imm << "(x" << rs1 << ")\n";
	    break;
	  }
	case 2:
	  {
	    cout << "\tSW\tx" << rd << ", " << S_imm << "(x" << rs1 << ")\n";
	    break;
	  }
	}
    }
  else if (opcode == 0x63)
    {
      int Itemp1_4 = (instWord >> 8) & 0xf;	//1111
      int Itemp5_10 = (instWord >> 25) & 0x3F;	//111111 (6-bits)
      int Itemp11 = (instWord >> 7) & 0x1;	// 1-bit
      int Itemp12 = (instWord >> 31) & 0x1;	// 1-bit
      I_imm =
	(Itemp11 << 11) | (Itemp1_4) | (Itemp5_10 << 5) | (Itemp12 << 12);

      int sign_bit = I_imm >> 12;
      if (sign_bit == 1)
	{
	  I_imm = I_imm - 8191 - 1;
	}

      switch (funct3)
	{
	case 0:
	    cout << "\tBEQ\tx" << rs1 << ", x" << rs2 << ", " << I_imm <<
	      "\n";
	  break;
	case 1:
	    cout << "\tBNE\tx" << rs1 << ", x" << rs2 << ", " << I_imm <<
	      "\n";
	  break;
	case 4:
	  cout << "\tBLT\tx" << rs1 << ", x" << rs2 << ", " << I_imm << "\n";
	  break;
	case 5:
	  cout << "\tBGE\tx" << rs1 << ", x" << rs2 << ", " << I_imm << "\n";
	  break;
	case 6:
	  cout << "\tBLTU\tx" << rs1 << ", x" << rs2 << ", " << I_imm << "\n";
	  break;
	case 7:
	  cout << "\tBGEU\tx" << rs1 << ", x" << rs2 << ", " << I_imm << "\n";
	  break;
	default:
	  cout << "\tUnkown SB Instruction \n";
	}
    }
  else				// u-instruction
  if (opcode == 0x37)
    {
      U_imm = (instWord >> 12);

      int sign_bit = U_imm >> 19;
      if (sign_bit == 1)
	{
	  U_imm = U_imm - 1048575 - 1;
	}

      cout << "\tLUI\tx" << rd << ", " << U_imm << "\n";
    }

  else if (opcode == 0x17)
    {
      U_imm = (instWord >> 12);

      int sign_bit = U_imm >> 19;
      if (sign_bit == 1)
	{
	  U_imm = U_imm - 1048575 - 1;
	}

      cout << "\tAUIPC\tx" << rd << ", " << U_imm << "\n";
    }
  // j- instruction    
  else if (opcode == 0x6f)
    {
      int Jtemp12_19 = (instWord >> 12) & 0xff;
      int Jtemp11 = (instWord >> 20) & 0x1;
      int Jtemp1_10 = (instWord >> 21) & 0x3ff;
      int Jtemp20 = (instWord >> 31) & 0x1;
      J_imm =
	(Jtemp12_19 << 12) | (Jtemp11 << 11) | (Jtemp1_10) | (Jtemp20 << 20);

      int sign_bit = J_imm >> 20;
      if (sign_bit == 1)
	{
	  J_imm = J_imm - 2097151 - 1;
	}

      cout << "\tJAL\tx" << rd << ", " << J_imm << "\n";
    }
  else if (opcode == 0x73)
    {
      int csr = (instWord >> 20);	// offset
      switch (funct3)
	{
	case 0:
	  funct7 = (instWord >> 20);
	  if (funct7 == 0)
	    {
	      cout << "\tECALL\n";
	    }
	  else
	    {
	      cout << "\tEBREAK\n";
	    }
	  break;

	case 1:
	  cout << "\tCSRRW\tx" << rd << ", " << csr << ", x" << rs1 << "\n";
	  break;
	case 2:
	  cout << "\tCSRRS\tx" << rd << ", " << csr << ", x" << rs1 << "\n";
	  break;
	case 3:
	  cout << "\tCSRRC\tx" << rd << ", " << csr << ", x" << rs1 << "\n";
	  break;

	case 5:
	  cout << "\tCSRRWI\tx" << rd << ", " << csr << ", " << rs1 << "\n";
	  break;
	case 6:
	  cout << "\tCSRRSI\tx" << rd << ", " << csr << ", " << rs1 << "\n";
	  break;
	case 7:
	  cout << "\tCSRRCI\tx" << rd << ", " << csr << ", " << rs1 << "\n";
	  break;
	}
	
    }
    else
    if (opcode == 0xF){
        int succ = (instWord >> 20);
        int pred = (instWord >> 23)& 0x7;

	    switch(funct3)
	    {        
	        case 0:
	        cout<< "\tFENCE\t" << pred <<", "<<succ<<"\n";
	        break;
	        case 1:
	        cout<<"\tFENCE.I\t" <<"\n";
	        break;
	    }

    } 

  else
    {
      cout << "\tUnkown Instruction \n";
    }

}

int
main (int argc, char *argv[])
{

  unsigned int instWord = 0;
  ifstream inFile;
  ofstream outFile;
  unsigned int pc = 0;

  if (argc < 2)
    emitError ("use: disasm <machine_code_file_name>\n");

  inFile.open (argv[1], ios::in | ios::binary | ios::ate);

  if (inFile.is_open ())
    {
      int fsize = inFile.tellg ();

      inFile.seekg (0, inFile.beg);
      if (!inFile.read ((char *) buffer, fsize))
	emitError ("Cannot read from input file\n");

      while (pc < fsize)
	{
	  instWord = (unsigned char) buffer[pc] |
	    (((unsigned char) buffer[pc + 1]) << 8) |
	    (((unsigned char) buffer[pc + 2]) << 16) |
	    (((unsigned char) buffer[pc + 3]) << 24);
	  pc += 4;
	  // remove the following line once you have a complete disassembler
	  //      if(pc==32) break;                       // stop when PC reached address 32
	  instDecExec (instWord);
	}


    }
  else
    emitError ("Cannot access input file\n");
}