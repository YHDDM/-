package com.example.a123.wifictrl;

import static android.app.PendingIntent.getActivity;
import static kotlin.jvm.internal.Reflection.function;

import android.annotation.SuppressLint;
import android.content.Intent;
import android.os.Bundle;

import androidx.annotation.NonNull;

import com.dlong.dl10netassistant.OnNetThreadListener;
import com.dlong.dl10netassistant.TcpClientThread;
import com.google.android.material.bottomnavigation.BottomNavigationItemView;
import com.google.android.material.bottomnavigation.BottomNavigationMenuView;
import com.google.android.material.bottomnavigation.BottomNavigationView;

import androidx.cardview.widget.CardView;
import androidx.fragment.app.FragmentManager;
import androidx.fragment.app.FragmentTransaction;
import androidx.appcompat.app.AppCompatActivity;
import android.util.Log;
import android.view.MenuItem;
import android.view.View;
import android.view.Window;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import com.example.a123.wifictrl.fragment.Fragment_four;
import com.example.a123.wifictrl.fragment.Fragment_one;
import com.example.a123.wifictrl.fragment.Fragment_three;
import com.example.a123.wifictrl.fragment.Fragment_two;

import org.jetbrains.annotations.NotNull;
import org.w3c.dom.Text;

import java.lang.reflect.Field;
import java.util.HashMap;
import java.util.Map;
import java.util.Objects;

public class MainActivity extends AppCompatActivity implements OnNetThreadListener{
    //实例化4个fragment
    private Fragment_one fragment_one;
    private Fragment_two fragment_two;
    private Fragment_three fragment_three;
    private Fragment_four fragment_four;
    private FragmentManager fragmentManager;

    TcpClientThread tcp;
    public boolean fan_flag;
    TextView txv_wd_1,txv_humi_1,txv_light_1,txv_co2_1,txv_pm2_5_1,txv_shake_1,txv_red_1,txv_gases_1;
    EditText edt_ip,edt_port;
    ImageView img_harm_gases_1,img_red_1,img_shake_1,img_connect;
    CardView card_red;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        //隐藏标题栏，一定要在setContentView之前
        requestWindowFeature(Window.FEATURE_NO_TITLE);//remove title bar  即隐藏标题栏
        getSupportActionBar().hide();// 隐藏ActionBar
        setContentView(R.layout.activity_main);
        BottomNavigationView navigation = (BottomNavigationView) findViewById(R.id.navigation);
        //disableShiftMode(navigation);
        navigation.setOnNavigationItemSelectedListener(mOnNavigationItemSelectedListener);
        fragmentManager = getSupportFragmentManager();

        Fragment_three wifiFragment = new Fragment_three();
        FragmentManager fragmentManager = getSupportFragmentManager();
        if(fragment_three != null) {
            FragmentTransaction fragmentTransaction = fragmentManager.beginTransaction();
            fragmentTransaction.replace(R.id.content, wifiFragment);
            fragmentTransaction.commit();
        }

        //设置默认显示页面
        setNavigationItemSelection(0);
        initView();
        try {
            fragment_one.btn_flag = 1;
            Intent intent = getIntent();
            String ipAddress = intent.getStringExtra("ipAddress");
            int portNumber = intent.getIntExtra("portNumber", 8899);
            tcp = new TcpClientThread(ipAddress, portNumber, this);
            Toast.makeText(getApplicationContext(), "连接成功", Toast.LENGTH_LONG).show();

//            fragment_four.txv_device_count.setText("3");
//            fragment_four.txv_channel.setText("Channel-25(2475MHz)");
//            fragment_four.txv_panid.setText("0x2401");
//            fragment_four.txv_host.setText("一班一组");

            tcp.start();
        } catch (Exception e) {
            Log.d("Err", "ip或者port为空");
        }
    }

    //初始化控件
    private void initView()
    {

    }


    //底部按钮点击监听，点击后实现页面显示
    private BottomNavigationView.OnNavigationItemSelectedListener mOnNavigationItemSelectedListener
            = new BottomNavigationView.OnNavigationItemSelectedListener() {

        @Override
        public boolean onNavigationItemSelected(@NonNull MenuItem item) {
            switch (item.getItemId()) {

                case R.id.navigation_hj:
                    setNavigationItemSelection(0);
                    return true;
                case R.id.navigation_cs:
                    setNavigationItemSelection(1);
                    return true;
                case R.id.navigation_dk:
                    setNavigationItemSelection(2);
                    return true;
                case R.id.navigation_dj:
                    setNavigationItemSelection(3);
                    return true;
            }
            return false;
        }

    };
    @SuppressLint("RestrictedApi")
    public static void disableShiftMode(BottomNavigationView view) {
        BottomNavigationMenuView menuView = (BottomNavigationMenuView)view.getChildAt(0);
        try {
            Field shiftingMode =
                    menuView.getClass().getDeclaredField("mShiftingMode");
            shiftingMode.setAccessible(true);
            shiftingMode.setBoolean(menuView, false);
            shiftingMode.setAccessible(false);
            for (int i = 0; i < menuView.getChildCount(); i++) {
                BottomNavigationItemView item = (BottomNavigationItemView) menuView.getChildAt(i);
                item.setShifting(false);
                item.setChecked(item.getItemData().isChecked());
            }
        } catch (NoSuchFieldException e) {
            Log.e("BNVHelper", "Unable to get shift mode field", e);
        } catch (IllegalAccessException e) {
            Log.e("BNVHelper", "Unable to change value of shift mode", e);
        }
    }


    //页面显示选择显示
    private void setNavigationItemSelection(int index) {
        FragmentTransaction transaction = fragmentManager.beginTransaction();
        hideFragment(transaction);
        switch (index) {
            case 0:
                if (fragment_one == null) {
                    fragment_one = new Fragment_one(tcp);
                    transaction.add(R.id.content, fragment_one);

                } else {
                    transaction.show(fragment_one);
                }
                //TODO setFragmentOneTextView()此方法测试 可删除

                break;
            case 1:
                if (fragment_two == null) {
                    fragment_two = new Fragment_two(tcp);
                    transaction.add(R.id.content, fragment_two);
                } else {
                    transaction.show(fragment_two);
                }
                break;
            case 2:
                if (fragment_three == null) {
                    fragment_three = new Fragment_three();
                    transaction.add(R.id.content, fragment_three);
                } else {
                    transaction.show(fragment_three);
                }
                break;
            case 3:
                if (fragment_four == null) {
                    fragment_four = new Fragment_four();
                    transaction.add(R.id.content, fragment_four);
                } else {
                    transaction.show(fragment_four);
                }
                break;
        }
        transaction.commit();
    }

    //隐藏不需要的页面
    private void hideFragment(FragmentTransaction transaction) {
        if (fragment_one != null) {
            transaction.hide(fragment_one);
        }
        if (fragment_two != null) {
            transaction.hide(fragment_two);
        }
        if (fragment_three != null) {
            transaction.hide(fragment_three);
        }
        if (fragment_four != null) {
            transaction.hide(fragment_four);
        }

    }


    @Override
    public void onAcceptSocket(@NotNull String s) {

    }

    @Override
    public void onConnectFailed(@NotNull String s) {

    }

    @Override
    public void onConnected(@NotNull String s) {

    }

    @Override
    public void onDisconnect(@NotNull String s) {

    }

    @Override
    public void onError(@NotNull String s, @NotNull String s1) {

    }

    @Override
    public void onReceive(@NotNull String s, int i, long l, @NotNull byte[] bytes) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                if(fragment_four!=null)
                {
                    fragment_four.txv_device_count.setText("3");
                    fragment_four.txv_channel.setText("Channel-25(2475MHz)");
                    fragment_four.txv_panid.setText("0x2401");
                    fragment_four.txv_host.setText("一班一组");
                }
            }
        });

        //Log.d("ReceiveHexStr",bytes2HexStr(bytes));
        try {
            Protocol pl = new Protocol(bytes);
            Map<String, Float> map = new HashMap<>();
            map = pl.getSensorValues(bytes);
            Map<String, Float> finalMap = map;
            if (fragment_one != null) {
                //byte[] bytes1={0x44,0x5a,0x4c,0x13,0x03,0x01,0x10,0x22,0x00,0x01,0x02,0x41,(byte)0xb3,0x16,0x03,0x22,0x00,0x02,0x02,0x41,0x4f,(byte)0x98,(byte)0x90,0x24};
                if (fragment_one.btn_flag == 1) {
                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            txv_wd_1 = (TextView) fragment_one.getView().findViewById(R.id.txv_wd_1);
                            txv_humi_1 = (TextView) fragment_one.getView().findViewById(R.id.txv_humi_1);
                            txv_light_1 = (TextView) fragment_one.getView().findViewById(R.id.txv_light_1);
                            txv_co2_1 = (TextView) fragment_one.getView().findViewById(R.id.txv_co2_1);
                            txv_pm2_5_1 = (TextView) fragment_one.getView().findViewById(R.id.txv_pm2_5_1);
                            txv_red_1 = (TextView) fragment_one.getView().findViewById(R.id.txv_red_1);
                            txv_shake_1 = (TextView) fragment_one.getView().findViewById(R.id.txv_shake_1);
                            txv_gases_1 = (TextView) fragment_one.getView().findViewById(R.id.txv_gases_1);

                            img_red_1 = (ImageView) fragment_one.getView().findViewById(R.id.img_red_1);
                            img_shake_1 = (ImageView) fragment_one.getView().findViewById(R.id.img_shake_1);
                            img_harm_gases_1 = (ImageView) fragment_one.getView().findViewById(R.id.img_harm_gases_1);
                            //img_connect = (ImageView) fragment_four.getView().findViewById(R.id.img_connect);
                            if (finalMap.size() > 0) {
                                if (finalMap.containsKey("卧室温度")) {
                                    if (Float.parseFloat((finalMap.get("卧室温度")).toString()) > 30.0) {
                                        //发送风扇开启指令
                                        tcp.send(new byte[]{0x44, 0x5a, 0x4c, 0x01, 0x04, 0x01, 0x02, 0x01, 0x01, 0x05});
                                        fan_flag = true;
                                    } else {
                                        //发送风扇关闭指令
                                        tcp.send(new byte[]{0x44, 0x5a, 0x4c, 0x01, 0x04, 0x01, 0x02, 0x01, 0x02, 0x06});
                                        fan_flag = false;
                                    }
                                    txv_wd_1.setText(String.format("%.1f", finalMap.get("卧室温度")));
                                }
                                if (finalMap.containsKey("卧室湿度")) {
                                    txv_humi_1.setText(String.format("%.1f", finalMap.get("卧室湿度")));
                                }

                                if (finalMap.containsKey("卧室光照强度")) {
                                    txv_light_1.setText(String.format("%.1f", finalMap.get("卧室光照强度")));
                                }
                                if (finalMap.containsKey("卧室PM2.5")) {
                                    txv_pm2_5_1.setText(String.format("%.1f", finalMap.get("卧室PM2.5")));
                                }

                                if (finalMap.containsKey("卧室CO2")) {
                                    txv_co2_1.setText(String.format("%.1f", finalMap.get("卧室CO2")));
                                }
                                if (finalMap.containsKey("卧室PM2.5")) {
                                    if (Float.parseFloat((finalMap.get("卧室PM2.5")).toString()) > 75) {
                                        txv_gases_1.setText("卧室PM2.5异常");
                                        img_harm_gases_1.setImageResource(R.drawable.high_harmful_gases);
                                    } else {
                                        txv_gases_1.setText("卧室PM2.5正常");
                                        img_harm_gases_1.setImageResource(R.drawable.normal_harmful_gases);
                                    }
                                }
                                if (finalMap.containsKey("卧室RED")) {
                                    txv_red_1.setText(String.format("%.1f", finalMap.get("卧室RED")));
                                }
                                if (finalMap.get("卧室RED") != null) {
                                    if (Float.parseFloat(Objects.requireNonNull(finalMap.get("卧室RED")).toString()) == 0.0) {
                                        txv_red_1.setText(String.format("卧室无人通过", finalMap.get("卧室RED")));
                                        img_red_1.setImageResource(R.drawable.normal_infrared);
                                    } else {
                                        txv_red_1.setText(String.format("卧室有人通过", finalMap.get("卧室RED")));
                                        img_red_1.setImageResource(R.drawable.high_infrared);
                                    }
                                }
                                if (finalMap.get("卧室SHAKE") != null) {
                                    if (Float.parseFloat(Objects.requireNonNull(finalMap.get("卧室SHAKE")).toString()) == 0.0) {
                                        txv_shake_1.setText(String.format("卧室玻璃正常", finalMap.get("卧室SHAKE")));
                                        img_shake_1.setImageResource(R.drawable.normal_shake);
                                    } else {
                                        txv_shake_1.setText(String.format("卧室玻璃异常", finalMap.get("卧室SHAKE")));
                                        img_shake_1.setImageResource(R.drawable.high_shake);
                                    }
                                }
                            }
                        }

                    });
                    Log.d("SensorValues", map.size() + "");

                } else if (fragment_one.btn_flag == 2) {

//                    Protocol pl = new Protocol(bytes);
//                    Map<String,Float> map = new HashMap<>();
//                    map = pl.getSensorValues(bytes);
//                    Map<String,Float> finalMap = map;
                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            txv_wd_1 = (TextView) fragment_one.getView().findViewById(R.id.txv_wd_1);
                            txv_humi_1 = (TextView) fragment_one.getView().findViewById(R.id.txv_humi_1);
                            txv_light_1 = (TextView) fragment_one.getView().findViewById(R.id.txv_light_1);
                            txv_co2_1 = (TextView) fragment_one.getView().findViewById(R.id.txv_co2_1);
                            txv_pm2_5_1 = (TextView) fragment_one.getView().findViewById(R.id.txv_pm2_5_1);
                            txv_red_1 = (TextView) fragment_one.getView().findViewById(R.id.txv_red_1);
                            txv_shake_1 = (TextView) fragment_one.getView().findViewById(R.id.txv_shake_1);
                            txv_gases_1 = (TextView) fragment_one.getView().findViewById(R.id.txv_gases_1);

                            img_red_1 = (ImageView) fragment_one.getView().findViewById(R.id.img_red_1);
                            img_shake_1 = (ImageView) fragment_one.getView().findViewById(R.id.img_shake_1);
                            img_harm_gases_1 = (ImageView) fragment_one.getView().findViewById(R.id.img_harm_gases_1);
                            //img_connect = (ImageView) fragment_four.getView().findViewById(R.id.img_connect);
                            if (finalMap.size() > 0) {
                                if (finalMap.containsKey("客厅温度"))
                                    txv_wd_1.setText(String.format("%.1f", finalMap.get("客厅温度")));
                                if (finalMap.get("客厅温度") != null) {
                                    if (Float.parseFloat(txv_wd_1.getText().toString()) > 30.0) {
                                        //发送风扇开启指令
                                        tcp.send(new byte[]{0x44, 0x5a, 0x4c, 0x02, 0x04, 0x01, 0x02, 0x01, 0x01, 0x05});
                                        fan_flag = true;
                                    } else {
                                        //发送风扇关闭指令
                                        tcp.send(new byte[]{0x44, 0x5a, 0x4c, 0x02, 0x04, 0x01, 0x02, 0x01, 0x02, 0x06});
                                        fan_flag = false;
                                    }
                                }
                                if (finalMap.containsKey("客厅湿度"))
                                    txv_humi_1.setText(String.format("%.1f", finalMap.get("客厅湿度")));
                                if (finalMap.containsKey("客厅光照强度"))
                                    txv_light_1.setText(String.format("%.1f", finalMap.get("客厅光照强度")));
                                if (finalMap.containsKey("客厅PM2.5"))
                                    txv_pm2_5_1.setText(String.format("%.1f", finalMap.get("客厅PM2.5")));
                                if (finalMap.containsKey("客厅CO2"))
                                    txv_co2_1.setText(String.format("%.1f", finalMap.get("客厅CO2")));
                                if (finalMap.containsKey("客厅PM2.5")) {
                                    if (Float.parseFloat((finalMap.get("客厅PM2.5")).toString()) > 75) {
                                        txv_gases_1.setText("客厅PM2.5异常");
                                        img_harm_gases_1.setImageResource(R.drawable.high_harmful_gases);
                                    } else {
                                        txv_gases_1.setText("客厅PM2.5正常");
                                        img_harm_gases_1.setImageResource(R.drawable.normal_harmful_gases);
                                    }

                                }
                                if (finalMap.containsKey("客厅RED"))
                                    txv_red_1.setText(String.format("%.1f", finalMap.get("客厅RED")));
                                if (finalMap.containsKey("客厅RED")) {
                                    if (Float.parseFloat(Objects.requireNonNull(finalMap.get("客厅RED")).toString()) == 0.0) {
                                        txv_red_1.setText(String.format("客厅无人通过", finalMap.get("客厅RED")));
                                        img_red_1.setImageResource(R.drawable.normal_infrared);
                                    } else {
                                        txv_red_1.setText(String.format("客厅有人通过", finalMap.get("客厅RED")));
                                        img_red_1.setImageResource(R.drawable.high_infrared);
                                    }
                                }
                                if (finalMap.get("客厅SHAKE") != null) {
                                    if (Float.parseFloat(Objects.requireNonNull(finalMap.get("客厅SHAKE")).toString()) == 0.0) {
                                        txv_shake_1.setText(String.format("客厅玻璃正常", finalMap.get("客厅SHAKE")));
                                        img_shake_1.setImageResource(R.drawable.normal_shake);
                                    } else {
                                        txv_shake_1.setText(String.format("客厅玻璃异常", finalMap.get("客厅SHAKE")));
                                        img_shake_1.setImageResource(R.drawable.high_shake);
                                    }
                                }
                            }
                        }

                    });
                    Log.d("SensorValues", map.size() + "");
                } else if (fragment_one.btn_flag == 3) {

//                    Protocol pl = new Protocol(bytes);
//                    Map<String,Float> map = new HashMap<>();
//                    map = pl.getSensorValues(bytes);
//                    Map<String,Float> finalMap = map;
                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            txv_wd_1 = (TextView) fragment_one.getView().findViewById(R.id.txv_wd_1);
                            txv_humi_1 = (TextView) fragment_one.getView().findViewById(R.id.txv_humi_1);
                            txv_light_1 = (TextView) fragment_one.getView().findViewById(R.id.txv_light_1);
                            txv_co2_1 = (TextView) fragment_one.getView().findViewById(R.id.txv_co2_1);
                            txv_pm2_5_1 = (TextView) fragment_one.getView().findViewById(R.id.txv_pm2_5_1);
                            txv_red_1 = (TextView) fragment_one.getView().findViewById(R.id.txv_red_1);
                            txv_shake_1 = (TextView) fragment_one.getView().findViewById(R.id.txv_shake_1);
                            txv_gases_1 = (TextView) fragment_one.getView().findViewById(R.id.txv_gases_1);

                            img_red_1 = (ImageView) fragment_one.getView().findViewById(R.id.img_red_1);
                            img_shake_1 = (ImageView) fragment_one.getView().findViewById(R.id.img_shake_1);
                            img_harm_gases_1 = (ImageView) fragment_one.getView().findViewById(R.id.img_harm_gases_1);
                            //img_connect = (ImageView) fragment_four.getView().findViewById(R.id.img_connect);
                            if (finalMap.size() > 0) {
                                if (finalMap.containsKey("室外温度")) {
                                    txv_wd_1.setText(String.format("%.1f", finalMap.get("室外温度")));
                                }
                                if (finalMap.containsKey("室外湿度")) {
                                    txv_humi_1.setText(String.format("%.1f", finalMap.get("室外湿度")));
                                }
                                if (finalMap.containsKey("室外光照强度")) {
                                    txv_light_1.setText(String.format("%.1f", finalMap.get("室外光照强度")));
                                }
                                if (finalMap.containsKey("室外PM2.5")) {
                                    txv_pm2_5_1.setText(String.format("%.1f", finalMap.get("室外PM2.5")));
                                }
                                if (finalMap.containsKey("室外CO2")) {
                                    txv_co2_1.setText(String.format("%.1f", finalMap.get("室外CO2")));
                                }
                                if (finalMap.containsKey("室外PM2.5")) {
                                    if (Float.parseFloat((finalMap.get("室外PM2.5")).toString()) > 75) {
                                        txv_gases_1.setText("室外PM2.5异常");
                                        img_harm_gases_1.setImageResource(R.drawable.high_harmful_gases);
                                    } else {
                                        txv_gases_1.setText("室外PM2.5正常");
                                        img_harm_gases_1.setImageResource(R.drawable.normal_harmful_gases);
                                    }

                                }
                                if (finalMap.containsKey("室外RED")) {
                                    txv_red_1.setText(String.format("%.1f", finalMap.get("室外RED")));
                                }
                                if (finalMap.containsKey("室外RED")) {
                                    if (Float.parseFloat(Objects.requireNonNull(finalMap.get("室外RED")).toString()) == 0.0) {
                                        txv_red_1.setText(String.format("室外无人通过", finalMap.get("室外RED")));
                                        img_red_1.setImageResource(R.drawable.normal_infrared);
                                    } else {
                                        txv_red_1.setText(String.format("室外有人通过", finalMap.get("室外RED")));
                                        img_red_1.setImageResource(R.drawable.high_infrared);
                                    }
                                }
                            }
                        }
                    });
                    Log.d("SensorValues", map.size() + "");
                }
            }
        } catch (Exception e) {
            Log.d("Err", e.getMessage().toString());
        }
    }
    public static String bytes2HexStr(byte[] bytes) {
        if (bytes == null) {
            return null;
        }
        StringBuilder b = new StringBuilder();
        for (int i = 0; i<bytes.length; i++) {
            b.append(String.format("%02x", bytes[i]& 0xFF));
        }
        return b.toString();
    }
}

