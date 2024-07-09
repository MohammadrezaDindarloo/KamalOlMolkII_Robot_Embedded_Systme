boolean comms_parseCommand(char* inS) {
  // strstr returns a pointer to the location of ",END" in the incoming string (inS).
  char* sub = strstr(inS, CMD_END);
  sub[strlen(CMD_END)] = 0;  // null terminate it directly after the ",END"
  if (strcmp(sub, CMD_END) == 0) {
    comms_extractParams(inS);
    return true;
  } else
    return false;
}

void comms_extractParams(char* inS) {
  char in[strlen(inS)];
  strcpy(in, inS);
  char* param;

  byte paramNumber = 0;
  param = strtok(in, COMMA);

  inParam1[0] = 0;
  inParam2[0] = 0;
  inParam3[0] = 0;
  inParam4[0] = 0;

  for (byte i = 0; i < 6; i++) {
    if (i == 0) strcpy(inCmd, param);
    else {
      param = strtok(NULL, COMMA);
      if (param != NULL) {
        if (strstr(CMD_END, param) == NULL) {
          // It's not null AND it wasn't 'END' either
          paramNumber++;
        }
      }

      switch (i) {
        case 1:
          if (param != NULL) strcpy(inParam1, param);
          break;
        case 2:
          if (param != NULL) strcpy(inParam2, param);
          break;
        case 3:
          if (param != NULL) strcpy(inParam3, param);
          break;
        case 4:
          if (param != NULL) strcpy(inParam4, param);
          break;
        default:
          break;
      }
    }
  }

  inNoOfParams = paramNumber;
}