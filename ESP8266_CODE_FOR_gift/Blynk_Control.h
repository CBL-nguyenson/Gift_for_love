
BLYNK_WRITE(Getdata_Temp) {
  float Var_Getdata_Temp = param.asDouble();
  Serial.print(Var_Getdata_Temp);
}

BLYNK_WRITE(Getdata_Hum) {
  float Var_Getdata_Hum = param.asDouble();
  Serial.print(Var_Getdata_Hum);
}
