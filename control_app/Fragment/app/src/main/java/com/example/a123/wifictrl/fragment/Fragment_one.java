package com.example.a123.wifictrl.fragment;


import android.content.Context;
import android.graphics.Color;
import android.media.Image;
import android.os.Bundle;
import androidx.annotation.IntegerRes;
import androidx.annotation.Nullable;
import androidx.cardview.widget.CardView;
import androidx.fragment.app.Fragment;

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
public class Fragment_one extends Fragment implements View.OnClickListener {

    ImageView img_shake_1,img_harm_gases_1,img_red_1;
    TextView txv_pm2_5_1,txv_shake_1,txv_red_1,txv_gases_1;
    Button room_btn,btn_livingroom,btn_outdoor;
    public TextView textView,textView15;
    public int btn_flag = 1;
//    CardView card_red;
    private View rootView;
    private Context mContext;

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        if (rootView == null) {
            rootView = inflater.inflate(R.layout.fragment_fragment_one, container, false);
            initView();

        } else {
            ViewGroup parent = (ViewGroup) rootView.getParent();
            if (parent != null) {
                parent.removeView(rootView);
            }
        }
        return rootView;
    }
    TcpClientThread _tcp;
    //初始化控件
    private void initView() {

        this.mContext = getActivity();
        //txv_wd = (TextView) rootView.findViewById(R.id.txv_wd);

//       card_red_1 = (CardView) rootView.findViewById(R.id.card_red_1);
        txv_pm2_5_1 = (TextView) rootView.findViewById(R.id.txv_pm2_5_1);
        txv_shake_1 = (TextView) rootView.findViewById(R.id.txv_shake_1);
        txv_red_1 = (TextView) rootView.findViewById(R.id.txv_red_1);
        txv_gases_1 = (TextView) rootView.findViewById(R.id.txv_gases_1);
        textView = (TextView) rootView.findViewById(R.id.textView);
        textView15 = (TextView) rootView.findViewById(R.id.textView15);
        img_shake_1 = (ImageView) rootView.findViewById(R.id.img_shake_1);
        img_harm_gases_1 = (ImageView) rootView.findViewById(R.id.img_harm_gases_1);
        img_red_1 = (ImageView) rootView.findViewById(R.id.img_red_1);
        room_btn = (Button) rootView.findViewById(R.id.room_btn);
        btn_livingroom = (Button) rootView.findViewById(R.id.btn_livingroom);
        btn_outdoor= (Button) rootView.findViewById(R.id.btn_outdoor);

        room_btn.setOnClickListener(this);
        btn_livingroom.setOnClickListener(this);
        btn_outdoor.setOnClickListener(this);
    }
    public Fragment_one(TcpClientThread tcp) {
        // Required empty public constructor
        //tcp发送数据
        _tcp = tcp;
        // _tcp.send(new byte[]{0x44,0x5a,0x4c,0x04,0x01,0x01,0x01,0x01,0x04});
        //改变按钮图片
    }

    @Override
    public void onClick(View view) {
        if(view.getId()==R.id.room_btn)
        {
            btn_flag = 1;
            textView.setText("卧室");
            room_btn.setBackgroundColor(Color.parseColor("#00C3FE"));
            btn_livingroom.setBackgroundColor(Color.parseColor("#c4c4c4"));
            btn_outdoor.setBackgroundColor(Color.parseColor("#c4c4c4"));
            textView15.setText("卧室情况");
        }
        else if(view.getId()==R.id.btn_livingroom)
        {
            btn_flag = 2;
            textView.setText("客厅");
            room_btn.setBackgroundColor(Color.parseColor("#c4c4c4"));
            btn_livingroom.setBackgroundColor(Color.parseColor("#00C3FE"));
            btn_outdoor.setBackgroundColor(Color.parseColor("#c4c4c4"));
            textView15.setText("客厅情况");
        } else if(view.getId()==R.id.btn_outdoor) {
            btn_flag = 3;
            textView.setText("室外");
            room_btn.setBackgroundColor(Color.parseColor("#c4c4c4"));
            btn_livingroom.setBackgroundColor(Color.parseColor("#c4c4c4"));
            btn_outdoor.setBackgroundColor(Color.parseColor("#00C3FE"));
            textView15.setText("室外情况");
        }
    }
}
