package com.example.a123.wifictrl.fragment;


import android.content.Context;
import android.graphics.Color;
import android.os.Bundle;
import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentManager;
import androidx.fragment.app.FragmentTransaction;

import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;

import com.dlong.dl10netassistant.TcpClientThread;
import com.example.a123.wifictrl.R;


/**
 * A simple {@link Fragment} subclass.
 */
public class Fragment_two extends Fragment implements View.OnClickListener {


    ImageView img_dg_1,img_dg_2,img_dg_3,img_dg_4,
            img_btn_dg_1,img_btn_dg_2,img_btn_dg_3,img_btn_dg_4,
            img_fan_1,img_fan_2,img_btn_fan_1,img_btn_fan_2,
            img_window,img_btn_window,img_glass,img_btn_glass;
    TextView textView2;
    public int flag_control = 1;
    Button room_btn,btn_livingroom,btn_outdoor;

    TcpClientThread _tcp;
    boolean dg_1_flag,dg_2_flag,dg_3_flag,dg_4_flag,fan_1_flag,fan_2_flag,window_flag,glass_flag;

    public Fragment_two(TcpClientThread tcp) {
        // Required empty public constructor
        //tcp发送数据
        _tcp = tcp;
       // _tcp.send(new byte[]{0x44,0x5a,0x4c,0x04,0x01,0x01,0x01,0x01,0x04});
        //改变按钮图片
    }
    private View rootView;

    private Context mContext;
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        if (rootView == null) {
            rootView = inflater.inflate(R.layout.fragment_fragment_two, container, false);
            initView();

        } else {
            ViewGroup parent = (ViewGroup) rootView.getParent();
            if (parent != null) {
                parent.removeView(rootView);
            }
        }
        return rootView;
    }

    private void initView() {
        this.mContext = getActivity();
        //txv_wd = (TextView) rootView.findViewById(R.id.txv_wd);
        img_dg_1 = (ImageView) rootView.findViewById(R.id.img_dg_1);
        img_dg_2 = (ImageView) rootView.findViewById(R.id.img_dg_2);
        img_dg_3 = (ImageView) rootView.findViewById(R.id.img_dg_3);
        img_dg_4 = (ImageView) rootView.findViewById(R.id.img_dg_4);

        img_btn_dg_1 = (ImageView) rootView.findViewById(R.id.img_btn_dg_1);
        img_btn_dg_2 = (ImageView) rootView.findViewById(R.id.img_btn_dg_2);
        img_btn_dg_3 = (ImageView) rootView.findViewById(R.id.img_btn_dg_3);
        img_btn_dg_4 = (ImageView) rootView.findViewById(R.id.img_btn_dg_4);

        textView2 = (TextView) rootView.findViewById(R.id.textView2);

        img_fan_1 = (ImageView) rootView.findViewById(R.id.img_fan_1);
        img_fan_2 = (ImageView) rootView.findViewById(R.id.img_fan_2);
        img_btn_fan_1 = (ImageView) rootView.findViewById(R.id.img_btn_fan_1);
        img_btn_fan_2 = (ImageView) rootView.findViewById(R.id.img_btn_fan_2);

        img_window = (ImageView) rootView.findViewById(R.id.img_window);
        img_btn_window = (ImageView) rootView.findViewById(R.id.img_btn_window);
        img_glass = (ImageView) rootView.findViewById(R.id.img_glass);
        img_btn_glass = (ImageView) rootView.findViewById(R.id.img_btn_glass);

        room_btn = (Button) rootView.findViewById(R.id.room_btn);
        btn_livingroom = (Button) rootView.findViewById(R.id.btn_livingroom);
        btn_outdoor= (Button) rootView.findViewById(R.id.btn_outdoor);

        btn_livingroom.setOnClickListener(this);
        btn_outdoor.setOnClickListener(this);

        img_btn_dg_1.setOnClickListener(this);
        img_btn_dg_2.setOnClickListener(this);
        img_btn_dg_3.setOnClickListener(this);
        img_btn_dg_4.setOnClickListener(this);
        img_btn_fan_1.setOnClickListener(this);
        img_btn_fan_2.setOnClickListener(this);
        img_btn_window.setOnClickListener(this);
        img_btn_glass.setOnClickListener(this);

        room_btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                flag_control = 1;
                img_btn_dg_1.setImageResource(R.drawable.crl_btn_off);
                img_btn_dg_2.setImageResource(R.drawable.crl_btn_off);
                img_btn_dg_3.setImageResource(R.drawable.crl_btn_off);
                img_btn_dg_4.setImageResource(R.drawable.crl_btn_off);
                img_btn_fan_1.setImageResource(R.drawable.crl_btn_off);
                img_btn_fan_2.setImageResource(R.drawable.crl_btn_off);
                img_btn_window.setImageResource(R.drawable.crl_btn_off);
                img_btn_glass.setImageResource(R.drawable.crl_btn_off);
                img_dg_1.setImageResource(R.drawable.crl_light_close);
                img_dg_2.setImageResource(R.drawable.crl_light_close);
                img_dg_3.setImageResource(R.drawable.crl_light_close);
                img_dg_4.setImageResource(R.drawable.crl_light_close);
                img_fan_1.setImageResource(R.drawable.fan_close);
                img_fan_2.setImageResource(R.drawable.fan_close);
                img_window.setImageResource(R.drawable.curtain_close);
                img_glass.setImageResource(R.drawable.glass_close);
                Log.d("step","1");
                textView2.setText("智能控制--卧室");
                room_btn.setBackgroundColor(Color.parseColor("#00C3FE"));
                btn_livingroom.setBackgroundColor(Color.parseColor("#c4c4c4"));
                btn_outdoor.setBackgroundColor(Color.parseColor("#c4c4c4"));
            }
        });
        btn_livingroom.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                flag_control = 2;
                img_btn_dg_1.setImageResource(R.drawable.crl_btn_off);
                img_btn_dg_2.setImageResource(R.drawable.crl_btn_off);
                img_btn_dg_3.setImageResource(R.drawable.crl_btn_off);
                img_btn_dg_4.setImageResource(R.drawable.crl_btn_off);
                img_btn_fan_1.setImageResource(R.drawable.crl_btn_off);
                img_btn_fan_2.setImageResource(R.drawable.crl_btn_off);
                img_btn_window.setImageResource(R.drawable.crl_btn_off);
                img_btn_glass.setImageResource(R.drawable.crl_btn_off);
                img_dg_1.setImageResource(R.drawable.crl_light_close);
                img_dg_2.setImageResource(R.drawable.crl_light_close);
                img_dg_3.setImageResource(R.drawable.crl_light_close);
                img_dg_4.setImageResource(R.drawable.crl_light_close);
                img_fan_1.setImageResource(R.drawable.fan_close);
                img_fan_2.setImageResource(R.drawable.fan_close);
                img_window.setImageResource(R.drawable.curtain_close);
                img_glass.setImageResource(R.drawable.glass_close);
                textView2.setText("智能控制--客厅");
                room_btn.setBackgroundColor(Color.parseColor("#c4c4c4"));
                btn_livingroom.setBackgroundColor(Color.parseColor("#00C3FE"));
                btn_outdoor.setBackgroundColor(Color.parseColor("#c4c4c4"));
            }
        });
        btn_outdoor.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                flag_control = 3;
                img_btn_dg_1.setImageResource(R.drawable.crl_btn_off);
                img_btn_dg_2.setImageResource(R.drawable.crl_btn_off);
                img_btn_dg_3.setImageResource(R.drawable.crl_btn_off);
                img_btn_dg_4.setImageResource(R.drawable.crl_btn_off);
                img_btn_fan_1.setImageResource(R.drawable.crl_btn_off);
                img_btn_fan_2.setImageResource(R.drawable.crl_btn_off);
                img_btn_window.setImageResource(R.drawable.crl_btn_off);
                img_btn_glass.setImageResource(R.drawable.crl_btn_off);
                img_dg_1.setImageResource(R.drawable.crl_light_close);
                img_dg_2.setImageResource(R.drawable.crl_light_close);
                img_dg_3.setImageResource(R.drawable.crl_light_close);
                img_dg_4.setImageResource(R.drawable.crl_light_close);
                img_fan_1.setImageResource(R.drawable.fan_close);
                img_fan_2.setImageResource(R.drawable.fan_close);
                img_window.setImageResource(R.drawable.curtain_close);
                img_glass.setImageResource(R.drawable.glass_close);
                textView2.setText("智能控制--室外");
                room_btn.setBackgroundColor(Color.parseColor("#c4c4c4"));
                btn_livingroom.setBackgroundColor(Color.parseColor("#c4c4c4"));
                btn_outdoor.setBackgroundColor(Color.parseColor("#00C3FE"));
            }
        });
    }

    @Override
    public void onClick(View view) {
        if(flag_control==1) {
            if (view.getId() == R.id.img_btn_dg_1) {
                if (!dg_1_flag) {
                    Log.d("step","2");
                    //发送继电器1开启指令
                    _tcp.send(new byte[]{0x44, 0x5a, 0x4c, 0x01, 0x04, 0x01, 0x01, 0x01, 0x01, 0x04});
                    //改变图片
                    img_btn_dg_1.setImageResource(R.drawable.crl_btn_on);
                    Log.d("step","3");
                    img_dg_1.setImageResource(R.drawable.crl_light_open);
                    Log.d("step","4");
                    dg_1_flag = true;
                } else {
                    //发送继电器1断开指令
                    _tcp.send(new byte[]{0x44, 0x5a, 0x4c, 0x01, 0x04, 0x01, 0x01, 0x01, 0x02, 0x05});
                    //改变图片
                    img_btn_dg_1.setImageResource(R.drawable.crl_btn_off);
                    img_dg_1.setImageResource(R.drawable.crl_light_close);
                    dg_1_flag = false;
                }
            } else if (view.getId() == R.id.img_btn_dg_2) {
                if (!dg_2_flag) {
                    //发送继电器2开启指令
                    _tcp.send(new byte[]{0x44, 0x5a, 0x4c, 0x01, 0x04, 0x01, 0x01, 0x02, 0x01, 0x05});
                    //改变图片
                    img_btn_dg_2.setImageResource(R.drawable.crl_btn_on);
                    img_dg_2.setImageResource(R.drawable.crl_light_open);
                    dg_2_flag = true;
                } else {
                    //发送继电器2断开指令
                    _tcp.send(new byte[]{0x44, 0x5a, 0x4c, 0x01, 0x04, 0x01, 0x01, 0x02, 0x02, 0x06});
                    //改变图片
                    img_btn_dg_2.setImageResource(R.drawable.crl_btn_off);
                    img_dg_2.setImageResource(R.drawable.crl_light_close);
                    dg_2_flag = false;
                }
            } else if (view.getId() == R.id.img_btn_dg_3) {
                if (!dg_3_flag) {
                    //发送继电器3开启指令
                    _tcp.send(new byte[]{0x44, 0x5a, 0x4c, 0x01, 0x04, 0x01, 0x01, 0x03, 0x01, 0x06});
                    //改变图片
                    img_btn_dg_3.setImageResource(R.drawable.crl_btn_on);
                    img_dg_3.setImageResource(R.drawable.crl_light_open);
                    dg_3_flag = true;
                } else {
                    //发送继电器3断开指令
                    _tcp.send(new byte[]{0x44, 0x5a, 0x4c, 0x01, 0x04, 0x01, 0x01, 0x03, 0x02, 0x07});
                    //改变图片
                    img_btn_dg_3.setImageResource(R.drawable.crl_btn_off);
                    img_dg_3.setImageResource(R.drawable.crl_light_close);
                    dg_3_flag = false;
                }
            } else if (view.getId() == R.id.img_btn_dg_4) {
                if (!dg_4_flag) {
                    //发送继电器4开启指令
                    _tcp.send(new byte[]{0x44, 0x5a, 0x4c, 0x01, 0x04, 0x01, 0x01, 0x04, 0x01, 0x07});
                    //改变图片
                    img_btn_dg_4.setImageResource(R.drawable.crl_btn_on);
                    img_dg_4.setImageResource(R.drawable.crl_light_open);
                    dg_4_flag = true;
                } else {
                    //发送继电器4断开指令
                    _tcp.send(new byte[]{0x44, 0x5a, 0x4c, 0x01, 0x04, 0x01, 0x01, 0x04, 0x02, 0x08});
                    //改变图片
                    img_btn_dg_4.setImageResource(R.drawable.crl_btn_off);
                    img_dg_4.setImageResource(R.drawable.crl_light_close);
                    dg_4_flag = false;
                }
            } else if (view.getId() == R.id.img_btn_fan_1) {
                if (!fan_1_flag) {
                    //发送风扇开启指令
                    _tcp.send(new byte[]{0x44, 0x5a, 0x4c, 0x01, 0x04, 0x01, 0x02, 0x01, 0x01, 0x05});
                    //改变图片
                    img_btn_fan_1.setImageResource(R.drawable.crl_btn_on);
                    img_fan_1.setImageResource(R.drawable.fan_open);
                    fan_1_flag = true;
                } else {
                    //发送风扇关闭指令
                    _tcp.send(new byte[]{0x44, 0x5a, 0x4c, 0x01, 0x04, 0x01, 0x02, 0x01, 0x02, 0x06});
                    //改变图片
                    img_btn_fan_1.setImageResource(R.drawable.crl_btn_off);
                    img_fan_1.setImageResource(R.drawable.fan_close);
                    fan_1_flag = false;
                }
            } else if (view.getId() == R.id.img_btn_fan_2) {
                if (!fan_2_flag) {
                    //发送风扇开启指令
                    _tcp.send(new byte[]{0x44, 0x5a, 0x4c, 0x01, 0x04, 0x01, 0x02, 0x02, 0x01, 0x06});
                    //改变图片
                    img_btn_fan_2.setImageResource(R.drawable.crl_btn_on);
                    img_fan_2.setImageResource(R.drawable.fan_open);
                    fan_2_flag = true;
                } else {
                    //发送风扇关闭指令
                    _tcp.send(new byte[]{0x44, 0x5a, 0x4c, 0x01, 0x04, 0x01, 0x02, 0x02, 0x02, 0x07});
                    //改变图片
                    img_btn_fan_2.setImageResource(R.drawable.crl_btn_off);
                    img_fan_2.setImageResource(R.drawable.fan_close);
                    fan_2_flag = false;
                }
            } else if (view.getId() == R.id.img_btn_window) {
                if (!window_flag) {
                    //发送风扇开启指令
                    _tcp.send(new byte[]{0x44, 0x5a, 0x4c, 0x01, 0x04, 0x01, 0x02, 0x02, 0x01, 0x06});
                    //改变图片
                    img_btn_window.setImageResource(R.drawable.crl_btn_on);
                    img_window.setImageResource(R.drawable.curtain_open);
                    window_flag = true;
                } else {
                    //发送风扇关闭指令
                    _tcp.send(new byte[]{0x44, 0x5a, 0x4c, 0x01, 0x04, 0x01, 0x02, 0x02, 0x02, 0x07});
                    //改变图片
                    img_btn_window.setImageResource(R.drawable.crl_btn_off);
                    img_window.setImageResource(R.drawable.curtain_close);
                    window_flag = false;
                }
            } else if (view.getId() == R.id.img_btn_glass) {
                if (!glass_flag) {
                    //发送风扇开启指令
                    _tcp.send(new byte[]{0x44, 0x5a, 0x4c, 0x01, 0x04, 0x01, 0x02, 0x02, 0x01, 0x06});
                    //改变图片
                    img_btn_glass.setImageResource(R.drawable.crl_btn_on);
                    img_glass.setImageResource(R.drawable.glass_open);
                    glass_flag = true;
                } else {
                    //发送风扇关闭指令
                    _tcp.send(new byte[]{0x44, 0x5a, 0x4c, 0x01, 0x04, 0x01, 0x02, 0x02, 0x02, 0x07});
                    //改变图片
                    img_btn_glass.setImageResource(R.drawable.crl_btn_off);
                    img_glass.setImageResource(R.drawable.glass_close);
                    glass_flag = false;
                }
            }
        }
        else if (flag_control == 2) {
                if (view.getId() == R.id.img_btn_dg_1) {
                    if (!dg_1_flag) {
                        //发送继电器1开启指令
                        _tcp.send(new byte[]{0x44, 0x5a, 0x4c, 0x02, 0x04, 0x01, 0x01, 0x01, 0x01, 0x04});
                        //改变图片
                        img_btn_dg_1.setImageResource(R.drawable.crl_btn_on);
                        img_dg_1.setImageResource(R.drawable.crl_light_open);
                        dg_1_flag = true;
                    } else {
                        //发送继电器1断开指令
                        _tcp.send(new byte[]{0x44, 0x5a, 0x4c, 0x02, 0x04, 0x01, 0x01, 0x01, 0x02, 0x05});
                        //改变图片
                        img_btn_dg_1.setImageResource(R.drawable.crl_btn_off);
                        img_dg_1.setImageResource(R.drawable.crl_light_close);
                        dg_1_flag = false;
                    }
                } else if (view.getId() == R.id.img_btn_dg_2) {
                    if (!dg_2_flag) {
                        //发送继电器2开启指令
                        _tcp.send(new byte[]{0x44, 0x5a, 0x4c, 0x02, 0x04, 0x01, 0x01, 0x02, 0x01, 0x05});
                        //改变图片
                        img_btn_dg_2.setImageResource(R.drawable.crl_btn_on);
                        img_dg_2.setImageResource(R.drawable.crl_light_open);
                        dg_2_flag = true;
                    } else {
                        //发送继电器2断开指令
                        _tcp.send(new byte[]{0x44, 0x5a, 0x4c, 0x02, 0x04, 0x01, 0x01, 0x02, 0x02, 0x06});
                        //改变图片
                        img_btn_dg_2.setImageResource(R.drawable.crl_btn_off);
                        img_dg_2.setImageResource(R.drawable.crl_light_close);
                        dg_2_flag = false;
                    }
                } else if (view.getId() == R.id.img_btn_dg_3) {
                    if (!dg_3_flag) {
                        //发送继电器3开启指令
                        _tcp.send(new byte[]{0x44, 0x5a, 0x4c, 0x02, 0x04, 0x01, 0x01, 0x03, 0x01, 0x06});
                        //改变图片
                        img_btn_dg_3.setImageResource(R.drawable.crl_btn_on);
                        img_dg_3.setImageResource(R.drawable.crl_light_open);
                        dg_3_flag = true;
                    } else {
                        //发送继电器3断开指令
                        _tcp.send(new byte[]{0x44, 0x5a, 0x4c, 0x02, 0x04, 0x01, 0x01, 0x03, 0x02, 0x07});
                        //改变图片
                        img_btn_dg_3.setImageResource(R.drawable.crl_btn_off);
                        img_dg_3.setImageResource(R.drawable.crl_light_close);
                        dg_3_flag = false;
                    }
                } else if (view.getId() == R.id.img_btn_dg_4) {
                    if (!dg_4_flag) {
                        //发送继电器4开启指令
                        _tcp.send(new byte[]{0x44, 0x5a, 0x4c, 0x02, 0x04, 0x01, 0x01, 0x04, 0x01, 0x07});
                        //改变图片
                        img_btn_dg_4.setImageResource(R.drawable.crl_btn_on);
                        img_dg_4.setImageResource(R.drawable.crl_light_open);
                        dg_4_flag = true;
                    } else {
                        //发送继电器4断开指令
                        _tcp.send(new byte[]{0x44, 0x5a, 0x4c, 0x02, 0x04, 0x01, 0x01, 0x04, 0x02, 0x08});
                        //改变图片
                        img_btn_dg_4.setImageResource(R.drawable.crl_btn_off);
                        img_dg_4.setImageResource(R.drawable.crl_light_close);
                        dg_4_flag = false;
                    }
                } else if (view.getId() == R.id.img_btn_fan_1) {
                    if (!fan_1_flag) {
                        //发送风扇开启指令
                        _tcp.send(new byte[]{0x44, 0x5a, 0x4c, 0x02, 0x04, 0x01, 0x02, 0x01, 0x01, 0x05});
                        //改变图片
                        img_btn_fan_1.setImageResource(R.drawable.crl_btn_on);
                        img_fan_1.setImageResource(R.drawable.fan_open);
                        fan_1_flag = true;
                    } else {
                        //发送风扇关闭指令
                        _tcp.send(new byte[]{0x44, 0x5a, 0x4c, 0x02, 0x04, 0x01, 0x02, 0x01, 0x02, 0x06});
                        //改变图片
                        img_btn_fan_1.setImageResource(R.drawable.crl_btn_off);
                        img_fan_1.setImageResource(R.drawable.fan_close);
                        fan_1_flag = false;
                    }
                } else if (view.getId() == R.id.img_btn_fan_2) {
                    if (!fan_2_flag) {
                        //发送风扇开启指令
                        _tcp.send(new byte[]{0x44, 0x5a, 0x4c, 0x02, 0x04, 0x01, 0x02, 0x02, 0x01, 0x06});
                        //改变图片
                        img_btn_fan_2.setImageResource(R.drawable.crl_btn_on);
                        img_fan_2.setImageResource(R.drawable.fan_open);
                        fan_2_flag = true;
                    } else {
                        //发送风扇关闭指令
                        _tcp.send(new byte[]{0x44, 0x5a, 0x4c, 0x02, 0x04, 0x01, 0x02, 0x02, 0x02, 0x07});
                        //改变图片
                        img_btn_fan_2.setImageResource(R.drawable.crl_btn_off);
                        img_fan_2.setImageResource(R.drawable.fan_close);
                        fan_2_flag = false;
                    }
                } else if (view.getId() == R.id.img_btn_window) {
                    if (!window_flag) {
                        //发送风扇开启指令
                        _tcp.send(new byte[]{0x44, 0x5a, 0x4c, 0x02, 0x04, 0x01, 0x02, 0x02, 0x01, 0x06});
                        //改变图片
                        img_btn_window.setImageResource(R.drawable.crl_btn_on);
                        img_window.setImageResource(R.drawable.curtain_open);
                        window_flag = true;
                    } else {
                        //发送风扇关闭指令
                        _tcp.send(new byte[]{0x44, 0x5a, 0x4c, 0x02, 0x04, 0x01, 0x02, 0x02, 0x02, 0x07});
                        //改变图片
                        img_btn_window.setImageResource(R.drawable.crl_btn_off);
                        img_window.setImageResource(R.drawable.curtain_close);
                        window_flag = false;
                    }
                } else if (view.getId() == R.id.img_btn_glass) {
                    if (!glass_flag) {
                        //发送风扇开启指令
                        _tcp.send(new byte[]{0x44, 0x5a, 0x4c, 0x02, 0x04, 0x01, 0x02, 0x02, 0x01, 0x06});
                        //改变图片
                        img_btn_glass.setImageResource(R.drawable.crl_btn_on);
                        img_glass.setImageResource(R.drawable.glass_open);
                        glass_flag = true;
                    } else {
                        //发送风扇关闭指令
                        _tcp.send(new byte[]{0x44, 0x5a, 0x4c, 0x02, 0x04, 0x01, 0x02, 0x02, 0x02, 0x07});
                        //改变图片
                        img_btn_glass.setImageResource(R.drawable.crl_btn_off);
                        img_glass.setImageResource(R.drawable.glass_close);
                        glass_flag = false;
                    }
                }
        }
        else if(flag_control==3) {
                if (view.getId() == R.id.img_btn_dg_1) {
                    if (!dg_1_flag) {
                        //发送继电器1开启指令
                        _tcp.send(new byte[]{0x44, 0x5a, 0x4c, 0x03, 0x04, 0x01, 0x01, 0x01, 0x01, 0x04});
                        //改变图片
                        img_btn_dg_1.setImageResource(R.drawable.crl_btn_on);
                        img_dg_1.setImageResource(R.drawable.crl_light_open);
                        dg_1_flag = true;
                    } else {
                        //发送继电器1断开指令
                        _tcp.send(new byte[]{0x44, 0x5a, 0x4c, 0x03, 0x04, 0x01, 0x01, 0x01, 0x02, 0x05});
                        //改变图片
                        img_btn_dg_1.setImageResource(R.drawable.crl_btn_off);
                        img_dg_1.setImageResource(R.drawable.crl_light_close);
                        dg_1_flag = false;
                    }
                } else if (view.getId() == R.id.img_btn_dg_2) {
                    if (!dg_2_flag) {
                        //发送继电器2开启指令
                        _tcp.send(new byte[]{0x44, 0x5a, 0x4c, 0x03, 0x04, 0x01, 0x01, 0x02, 0x01, 0x05});
                        //改变图片
                        img_btn_dg_2.setImageResource(R.drawable.crl_btn_on);
                        img_dg_2.setImageResource(R.drawable.crl_light_open);
                        dg_2_flag = true;
                    } else {
                        //发送继电器2断开指令
                        _tcp.send(new byte[]{0x44, 0x5a, 0x4c, 0x03, 0x04, 0x01, 0x01, 0x02, 0x02, 0x06});
                        //改变图片
                        img_btn_dg_2.setImageResource(R.drawable.crl_btn_off);
                        img_dg_2.setImageResource(R.drawable.crl_light_close);
                        dg_2_flag = false;
                    }
                } else if (view.getId() == R.id.img_btn_dg_3) {
                    if (!dg_3_flag) {
                        //发送继电器3开启指令
                        _tcp.send(new byte[]{0x44, 0x5a, 0x4c, 0x03, 0x04, 0x01, 0x01, 0x03, 0x01, 0x06});
                        //改变图片
                        img_btn_dg_3.setImageResource(R.drawable.crl_btn_on);
                        img_dg_3.setImageResource(R.drawable.crl_light_open);
                        dg_3_flag = true;
                    } else {
                        //发送继电器3断开指令
                        _tcp.send(new byte[]{0x44, 0x5a, 0x4c, 0x03, 0x04, 0x01, 0x01, 0x03, 0x02, 0x07});
                        //改变图片
                        img_btn_dg_3.setImageResource(R.drawable.crl_btn_off);
                        img_dg_3.setImageResource(R.drawable.crl_light_close);
                        dg_3_flag = false;
                    }
                } else if (view.getId() == R.id.img_btn_dg_4) {
                    if (!dg_4_flag) {
                        //发送继电器4开启指令
                        _tcp.send(new byte[]{0x44, 0x5a, 0x4c, 0x03, 0x04, 0x01, 0x01, 0x04, 0x01, 0x07});
                        //改变图片
                        img_btn_dg_4.setImageResource(R.drawable.crl_btn_on);
                        img_dg_4.setImageResource(R.drawable.crl_light_open);
                        dg_4_flag = true;
                    } else {
                        //发送继电器4断开指令
                        _tcp.send(new byte[]{0x44, 0x5a, 0x4c, 0x03, 0x04, 0x01, 0x01, 0x04, 0x02, 0x08});
                        //改变图片
                        img_btn_dg_4.setImageResource(R.drawable.crl_btn_off);
                        img_dg_4.setImageResource(R.drawable.crl_light_close);
                        dg_4_flag = false;
                    }
                } else if (view.getId() == R.id.img_btn_fan_1) {
                    if (!fan_1_flag) {
                        //发送风扇开启指令
                        _tcp.send(new byte[]{0x44, 0x5a, 0x4c, 0x03, 0x04, 0x01, 0x02, 0x01, 0x01, 0x05});
                        //改变图片
                        img_btn_fan_1.setImageResource(R.drawable.crl_btn_on);
                        img_fan_1.setImageResource(R.drawable.fan_open);
                        fan_1_flag = true;
                    } else {
                        //发送风扇关闭指令
                        _tcp.send(new byte[]{0x44, 0x5a, 0x4c, 0x03, 0x04, 0x01, 0x02, 0x01, 0x02, 0x06});
                        //改变图片
                        img_btn_fan_1.setImageResource(R.drawable.crl_btn_off);
                        img_fan_1.setImageResource(R.drawable.fan_close);
                        fan_1_flag = false;
                    }
                } else if (view.getId() == R.id.img_btn_fan_2) {
                    if (!fan_2_flag) {
                        //发送风扇开启指令
                        _tcp.send(new byte[]{0x44, 0x5a, 0x4c, 0x03, 0x04, 0x01, 0x02, 0x02, 0x01, 0x06});
                        //改变图片
                        img_btn_fan_2.setImageResource(R.drawable.crl_btn_on);
                        img_fan_2.setImageResource(R.drawable.fan_open);
                        fan_2_flag = true;
                    } else {
                        //发送风扇关闭指令
                        _tcp.send(new byte[]{0x44, 0x5a, 0x4c, 0x03, 0x04, 0x01, 0x02, 0x02, 0x02, 0x07});
                        //改变图片
                        img_btn_fan_2.setImageResource(R.drawable.crl_btn_off);
                        img_fan_2.setImageResource(R.drawable.fan_close);
                        fan_2_flag = false;
                    }
                } else if (view.getId() == R.id.img_btn_window) {
                    if (!window_flag) {
                        //发送风扇开启指令
                        _tcp.send(new byte[]{0x44, 0x5a, 0x4c, 0x03, 0x04, 0x01, 0x02, 0x02, 0x01, 0x06});
                        //改变图片
                        img_btn_window.setImageResource(R.drawable.crl_btn_on);
                        img_window.setImageResource(R.drawable.curtain_open);
                        window_flag = true;
                    } else {
                        //发送风扇关闭指令
                        _tcp.send(new byte[]{0x44, 0x5a, 0x4c, 0x03, 0x04, 0x01, 0x02, 0x02, 0x02, 0x07});
                        //改变图片
                        img_btn_window.setImageResource(R.drawable.crl_btn_off);
                        img_window.setImageResource(R.drawable.curtain_close);
                        window_flag = false;
                    }
                } else if (view.getId() == R.id.img_btn_glass) {
                    if (!glass_flag) {
                        //发送风扇开启指令
                        _tcp.send(new byte[]{0x44, 0x5a, 0x4c, 0x03, 0x04, 0x01, 0x02, 0x02, 0x01, 0x06});
                        //改变图片
                        img_btn_glass.setImageResource(R.drawable.crl_btn_on);
                        img_glass.setImageResource(R.drawable.glass_open);
                        glass_flag = true;
                    } else {
                        //发送风扇关闭指令
                        _tcp.send(new byte[]{0x44, 0x5a, 0x4c, 0x03, 0x04, 0x01, 0x02, 0x02, 0x02, 0x07});
                        //改变图片
                        img_btn_glass.setImageResource(R.drawable.crl_btn_off);
                        img_glass.setImageResource(R.drawable.glass_close);
                        glass_flag = false;
                    }
                }
        }
    }
}
