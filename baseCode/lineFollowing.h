#ifndef LINEFOLLOWING
#define LINEFOLLOWING

// Populates the sensors[] variable so that we know amountSeen
void ReadLine() {
  amountSeen = 0;
  lastLineIndex = -1;
  for(int i = 7; i >= 0; --i) {

    sensors[i] = digitalRead(LINE_SENSOR[i]);

    if(sensors[i] == HIGH) {
      if(lastLineIndex == -1) {
        lastLineIndex = i;
      }
      ++amountSeen;
      firstLineIndex = i;
    }
  }

  sensorCounter = 0;
  //display.sendNum((amountSeen * 100) +(lastLineIndex)*10 + firstLineIndex);
}

void WriteWheelDirection(bool ldir, bool rdir) {
  digitalWrite(WHEEL_DIR_LF, ldir);
  digitalWrite(WHEEL_DIR_LB, !ldir);
  digitalWrite(WHEEL_DIR_RF, rdir);
  digitalWrite(WHEEL_DIR_RB, !rdir);
}

void WriteToWheels(int ls, int rs) {
  digitalWrite(H_BRIDGE_ENABLE,HIGH);
  if(ls < 0) {
    digitalWrite(WHEEL_DIR_LF, false); //right backwards
    digitalWrite(WHEEL_DIR_LB, true);
  }
  else {
    digitalWrite(WHEEL_DIR_LF, true); //right forwards
    digitalWrite(WHEEL_DIR_LB, false);
  }
  if(rs < 0) {
    digitalWrite(WHEEL_DIR_RF, false); //right backwards
    digitalWrite(WHEEL_DIR_RB, true);
  }
  else {
    digitalWrite(WHEEL_DIR_RF, true); //right forwards
    digitalWrite(WHEEL_DIR_RB, false);
  }

  //Make sure it does not exceed max
  if(ls > 255){ls = 255;} 
  if(rs > 255){rs = 255;}
  if(rs < -255){rs = -255;}
  if(ls < -255){ls = -255;} 

  analogWrite(WHEEL_SPEED_L, abs(ls));
  analogWrite(WHEEL_SPEED_R, abs(rs));
}

bool LineFollow(int ts, int strictness, int cen1 = 4, int cen2 = 3) {
  if (amountSeen == 0) {
    //floor it and pray? Consider Circling 
    WriteToWheels(ts, ts);
  }else{
    int rightSpeed = ts + (lastLineIndex - cen1) * strictness;
    int leftSpeed = ts - (firstLineIndex - cen2) * strictness;
    //display.sendNum( (leftSpeed*1000) + (rightSpeed * 100) + (lastLineIndex)*10 + firstLineIndex);
    WriteToWheels(leftSpeed, rightSpeed);
  }
  return false;
}

void favorLineFollow(int ts, int strictness, bool favorRight = false, int cen = 3,int stable = -7){
  //Sees no lines, use what the center is to guess a direction to turn
  if(amountSeen == 0){
    if(cen<4){//Turn left
      WriteToWheels(ts+4*strictness,ts-4*strictness);
    }else{//Turn right
      WriteToWheels(ts-4*strictness,ts+4*strictness);
    }
    return;
  }
  //-7 is a sentitent value for same as cen
  if(stable==-7) stable=cen;
  //Diff is the difference between the central index (cen) and the index of the line it's following
  int diff;
  if(favorRight)
    if(abs(lastLineIndex - cen)>abs(lastLineIndex - stable)) diff = lastLineIndex - stable;
    else diff = lastLineIndex - cen;
  else
    if(abs(firstLineIndex - cen)>abs(firstLineIndex - stable)) diff = firstLineIndex - stable;
    else diff = firstLineIndex - cen;
  
  //If diff is negative, it will turn to the left, if postive: turn left
  int rightSpeed = ts + diff * strictness;
  int leftSpeed  = ts - diff * strictness;
  WriteToWheels(leftSpeed,rightSpeed);
}

#endif
