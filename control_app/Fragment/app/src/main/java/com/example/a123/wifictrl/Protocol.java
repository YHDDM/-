package com.example.a123.wifictrl;

import android.util.Log;

import java.io.ByteArrayInputStream;
import java.io.DataInputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

public class Protocol {
    byte[] head;//帧头
    byte length;//长度
    byte addr;//地址
    byte[] data;//数据
    byte checksum;//校验


    public Protocol(byte[] receive) {
        head = new byte[]{receive[0], receive[1], receive[2]};
        length = receive[4];
        addr = receive[3];
        data = new byte[receive.length - 6];
        for (int i = 5; i < receive.length - 1; i++) {
            data[i - 5] = receive[i];
        }
        checksum = receive[receive.length - 1];
    }

    private boolean checkall(byte[] receive) {
        if (checkHead() && checklength(receive) && checkSum())
            return true;
        else
            return false;
    }

    //检查帧头
    private boolean checkHead() {
        if (head[0] == 0x44 && head[1] == 0x5a && head[2] == 0x4c && (addr == 0x01 || addr == 0x02 || addr == 0x03))
            return true;
        else
            return false;
    }

    //检查长度
    private boolean checklength(byte[] receive) {
        if (length == receive.length - 6)
            return true;
        else
            return false;
    }

    //检查校验和
    private byte sum(byte[] data) {
        byte result = 0;
        for (byte a : data) {
            result += a;
        }
        return result;
    }

    private boolean checkSum() {
        if (sum(data) == checksum)
            return true;
        else
            return false;
    }

    //拆分传感器数据
    private ArrayList<byte[]> getSensorBytes() {
        ArrayList<byte[]> sensorBytes = new ArrayList<>();
        //判断功能码和传感器
        if (data[0] == 0x03 && data[1] == 0x01) {
            //传感器上报数据
            //循环取出每组数据
            for (int i = 0; i < data[2] / 8; i++) {
                byte[] temp = new byte[8];
                for (int j = 0; j < 8; j++) {
                    temp[j] = data[i * 8 + 3 + j];
                }
                sensorBytes.add(temp);
            }
        }
        return sensorBytes;
    }

    public Map<String, Float> getSensorValues(byte[] receive) {
        Map<String, Float> map = new HashMap<>();
        if (checkall(receive)) {
            Log.d("temp", "1");
            ArrayList<byte[]> sensorBytes = getSensorBytes();
            for (byte[] sensor : sensorBytes) {
                Log.d("temp", "2");
                //判断环境参数
                if (receive[3] == 0x01) {
                    if (sensor[0] == 0x22 && sensor[1] == 0x00) {
                        switch (sensor[2]) {
                            //温度
                            case 0x01: {
                                if (sensor[3] == 0x02) {
                                    Log.d("temp", "3");
                                    //正常工作
                                    map.put("卧室温度", bytetofloat(new byte[]{sensor[4], sensor[5], sensor[6], sensor[7]}));
                                    Log.d("temp", "1_4");
                                }
                            }
                            break;
                            //湿度
                            case 0x02: {
                                if (sensor[3] == 0x02) {
                                    //正常工作
                                    map.put("卧室湿度", bytetofloat(new byte[]{sensor[4], sensor[5], sensor[6], sensor[7]}));
                                    Log.d("temp", "1_5");
                                }
                            }
                            break;
                            //震动
                            case 0x03: {
                                map.put("卧室SHAKE", bytetofloat(new byte[]{sensor[4], sensor[5], sensor[6], sensor[7]}));
                                Log.d("temp", "1_6");
                            }
                            break;
                            //光照强度
                            case 0x04: {
                                if (sensor[3] == 0x02) {
                                    //正常工作
                                    map.put("卧室光照强度", bytetofloat(new byte[]{sensor[4], sensor[5], sensor[6], sensor[7]}));
                                    Log.d("temp", "1_7");
                                }
                            }
                            break;
                            //CO2
                            case 0x05: {
                                if (sensor[3] == 0x02) {
                                    //正常工作
                                    map.put("卧室CO2", bytetofloat(new byte[]{sensor[4], sensor[5], sensor[6], sensor[7]}));
                                    Log.d("temp", "1_8");
                                }
                            }
                            break;
                            //PM2_5
                            case 0x06: {
                                if (sensor[3] == 0x02) {
                                    //正常工作
                                    map.put("卧室PM2.5", bytetofloat(new byte[]{sensor[4], sensor[5], sensor[6], sensor[7]}));
                                    Log.d("temp", "1_9");
                                }
                            }
                            break;
                            //RED
                            case 0x07: {
                                map.put("卧室RED", bytetofloat(new byte[]{sensor[4], sensor[5], sensor[6], sensor[7]}));
                                Log.d("temp", "1_10");
                            }
                            break;
                        }
                    }
                } else if(receive[3]==0x02) {
                    if (sensor[0] == 0x22 && sensor[1] == 0x00) {
                        switch (sensor[2]) {
                            //温度
                            case 0x01: {
                                if (sensor[3] == 0x02) {
                                    Log.d("temp", "2_3");
                                    //正常工作
                                    map.put("客厅温度", bytetofloat(new byte[]{sensor[4], sensor[5], sensor[6], sensor[7]}));
                                    Log.d("temp", "2_4");
                                }
                            }
                            break;
                            //湿度
                            case 0x02: {
                                if (sensor[3] == 0x02) {
                                    //正常工作
                                    map.put("客厅湿度", bytetofloat(new byte[]{sensor[4], sensor[5], sensor[6], sensor[7]}));
                                    Log.d("temp", "2_5");
                                }
                            }
                            break;
                            //震动
                            case 0x03: {
                                map.put("客厅SHAKE", bytetofloat(new byte[]{sensor[4], sensor[5], sensor[6], sensor[7]}));
                                Log.d("temp", "2_6");
                            }
                            break;
                            //光照强度
                            case 0x04: {
                                if (sensor[3] == 0x02) {
                                    //正常工作
                                    map.put("客厅光照强度", bytetofloat(new byte[]{sensor[4], sensor[5], sensor[6], sensor[7]}));
                                    Log.d("temp", "2_7");
                                }
                            }
                            break;
                            //CO2
                            case 0x05: {
                                if (sensor[3] == 0x02) {
                                    //正常工作
                                    map.put("客厅CO2", bytetofloat(new byte[]{sensor[4], sensor[5], sensor[6], sensor[7]}));
                                    Log.d("temp", "2_8");
                                }
                            }
                            break;
                            //PM2_5
                            case 0x06: {
                                if (sensor[3] == 0x02) {
                                    //正常工作
                                    map.put("客厅PM2.5", bytetofloat(new byte[]{sensor[4], sensor[5], sensor[6], sensor[7]}));
                                    Log.d("temp", "2_9");
                                }
                            }
                            break;
                            //RED
                            case 0x07: {
                                map.put("客厅RED", bytetofloat(new byte[]{sensor[4], sensor[5], sensor[6], sensor[7]}));
                                Log.d("temp", "2_10");
                            }
                            break;
                        }
                    }
                } else if(receive[3]==0x03) {
                    if (sensor[0] == 0x22 && sensor[1] == 0x00) {
                        switch (sensor[2]) {
                            //温度
                            case 0x01: {
                                if (sensor[3] == 0x02) {
                                    Log.d("temp", "3");
                                    //正常工作
                                    map.put("室外温度", bytetofloat(new byte[]{sensor[4], sensor[5], sensor[6], sensor[7]}));
                                    Log.d("temp", "3_4");
                                }
                            }
                            break;
                            //湿度
                            case 0x02: {
                                if (sensor[3] == 0x02) {
                                    //正常工作
                                    map.put("室外湿度", bytetofloat(new byte[]{sensor[4], sensor[5], sensor[6], sensor[7]}));
                                    Log.d("temp", "3_5");
                                }
                            }
                            break;
                            //震动
                            case 0x03: {
                                map.put("室外SHAKE", bytetofloat(new byte[]{sensor[4], sensor[5], sensor[6], sensor[7]}));
                                Log.d("temp", "3_6");
                            }
                            break;
                            //光照强度
                            case 0x04: {
                                if (sensor[3] == 0x02) {
                                    //正常工作
                                    map.put("室外光照强度", bytetofloat(new byte[]{sensor[4], sensor[5], sensor[6], sensor[7]}));
                                    Log.d("temp", "3_7");
                                }
                            }
                            break;
                            //CO2
                            case 0x05: {
                                if (sensor[3] == 0x02) {
                                    //正常工作
                                    map.put("室外CO2", bytetofloat(new byte[]{sensor[4], sensor[5], sensor[6], sensor[7]}));
                                    Log.d("temp", "3_8");
                                }
                            }
                            break;
                            //PM2_5
                            case 0x06: {
                                if (sensor[3] == 0x02) {
                                    //正常工作
                                    map.put("室外PM2.5", bytetofloat(new byte[]{sensor[4], sensor[5], sensor[6], sensor[7]}));
                                    Log.d("temp", "3_9");
                                }
                            }
                            break;
                            //RED
                            case 0x07: {
                                map.put("室外RED", bytetofloat(new byte[]{sensor[4], sensor[5], sensor[6], sensor[7]}));
                                Log.d("temp", "3_10");
                            }
                            break;
                        }
                    }
                }
            }
        }
        return map;
    }


    private Float bytetofloat(byte[] bys)
    {
        float data = 0;
        ByteArrayInputStream byteArrayInputStream =
                new ByteArrayInputStream(bys);
        DataInputStream dataInputStream =
                new DataInputStream(byteArrayInputStream);
        try {
            data = dataInputStream.readFloat();
        } catch (IOException e) {
            e.printStackTrace();
        }
        return data;
    }
}
