typedef enum _INSTRUCTION
{
  yes,
  no,
  left,
  right,
  i_end
} INSTRUCTION;

String iType[] = { "y","n","l","r"};

INSTRUCTION matchInstruct(String i){
  
  for(int c = yes; c != i_end; c++)
  {
    if(iType[c] == i)
    {
        return c;
    }
  }
  return i_end;
}

typedef enum _POSITION
{
  START,
  CORRIDOOR,
  L_ROOM,
  R_ROOM,
  L_CORNER,
  R_CORNER,
  L_R_CORNER,
  END,
  p_end

} POSITION;

typedef enum _MODE
{
  start,
  mode1,
  mode2,
  mode3,
  m_end
} MODE;

String mType[] = { "start","mode1", "mode2", "mode3" };

MODE matchMode(String mode){

  for(int m = mode1; m != m_end; m++)
  {
    if(mType[m] == mode)
    {
        return m;
    }
  }

  return m_end;
  
}