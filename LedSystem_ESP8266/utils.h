#ifndef UTILS_H
#define UTILS_H

int StrToInt(String value)
{
  if(value.length()>0 && value[0]=='-')
  {
    return -value.substring(1, value.length()).toInt();
  }
  return value.toInt();
}

int htmlColorToInt(String color)
{
  char charColor[9];
  String strColor = "0x"+color.substring(1, color.length());
  strColor.toCharArray(charColor, 9);
  return (int)strtol(charColor, NULL, 16);
}


String CRGB2Str(CRGB color)
{
  return "("+String(color.r)+", "+String(color.g)+", "+String(color.b)+")";
}


#endif // UTILS_H
