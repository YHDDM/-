package com.example.a123.wifictrl.fragment;


import android.content.Context;
import android.content.Intent;
import android.os.Bundle;

import androidx.cardview.widget.CardView;
import androidx.fragment.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;

import com.dlong.dl10netassistant.TcpClientThread;
import com.example.a123.wifictrl.MainActivity;
import com.example.a123.wifictrl.R;


/**
 * A simple {@link Fragment} subclass.
 */
public class Fragment_four extends Fragment implements View.OnClickListener {


    private View rootView;
    private Context mContext;

    EditText edt_ip,edt_port;
    ImageView img_connect;
    public TextView txv_device_count,txv_channel,txv_panid,txv_host;
    public Fragment_four() {
        // Required empty public constructor
    }

    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        if (rootView == null) {
            rootView = inflater.inflate(R.layout.fragment_fragment__four, container, false);
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
        edt_ip = (EditText) rootView.findViewById(R.id.edt_ip);
        edt_port = (EditText) rootView.findViewById(R.id.edt_port);
        img_connect = (ImageView) rootView.findViewById(R.id.img_connect);
        txv_device_count = (TextView) rootView.findViewById(R.id.txv_device_count);
        txv_channel = (TextView) rootView.findViewById(R.id.txv_channel);
        txv_panid = (TextView) rootView.findViewById(R.id.txv_panid);
        txv_host = (TextView) rootView.findViewById(R.id.txv_host);
        img_connect.setOnClickListener(this);
    }
    @Override
    public void onClick(View view) {
        if(view.getId()==R.id.img_connect)
        {

            Context context = getActivity();

            // 获取IP地址和端口号编辑框中的数据
            String ipAddress = edt_ip.getText().toString();
            int portNumber = Integer.parseInt(edt_port.getText().toString());

            // 创建一个Intent对象
            Intent intent = new Intent(context, MainActivity.class);

            // 将IP地址和端口号数据作为Extra传递给主页面
            intent.putExtra("ipAddress", ipAddress);
            intent.putExtra("portNumber", portNumber);
            startActivity(intent);
        }
    }
}
