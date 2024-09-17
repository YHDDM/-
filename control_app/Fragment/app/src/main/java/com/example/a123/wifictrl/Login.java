package com.example.a123.wifictrl;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Context;
import android.content.Intent;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.os.Bundle;
import android.text.method.HideReturnsTransformationMethod;
import android.text.method.PasswordTransformationMethod;
import android.util.Log;
import android.view.View;
import android.view.Window;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import java.sql.SQLException;

public class Login extends AppCompatActivity implements View.OnClickListener {

    ImageView imv_dl,pass_hide;
    TextView txv_zc;
    EditText edt_password,edt_account;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        //隐藏标题栏，一定要在setContentView之前
        requestWindowFeature(Window.FEATURE_NO_TITLE);//remove title bar  即隐藏标题栏
        getSupportActionBar().hide();// 隐藏ActionBar
        setContentView(R.layout.activity_login);
        imv_dl = findViewById(R.id.imv_dl);
        pass_hide = findViewById(R.id.pass_hide);
        edt_password = findViewById(R.id.edt_password);
        txv_zc = findViewById(R.id.txv_zc);
        edt_account = findViewById(R.id.edt_account);
        txv_zc.setOnClickListener(this);
        imv_dl.setOnClickListener(this);
        pass_hide.setOnClickListener(this);
    }


    int i = 0;
    @Override
    public void onClick(View view) {
        if(view.getId()==R.id.imv_dl)//登录触发
        {
            if(!edt_account.getText().toString().equals("")&&!edt_password.getText().toString().equals("")) {
                Thread thread = new Thread(new Runnable() {
                    @Override
                    public void run() {
                        SqlServer sql = new SqlServer("test", "Abc123456", "ShanDongZhao", "msg.comingthings.com:1433");
                        sql.connect();
                        try {
                            //通过用户输入的用户名查询对应的正确密码
                            String Password = sql.Select("SELECT Password FROM [User] where UserName='" + edt_account.getText().toString() + "'", "Password");
                            //判断用户输入的密码和正确密码是否一致
                            if (Password.equals(edt_password.getText().toString())) {
                                //如果一致,打开主页面
                                Intent it = new Intent(getApplicationContext(), MainActivity.class);
                                startActivity(it);
                            } else {
                                //不一致，弹出提示
                                runOnUiThread(new Runnable() {
                                    @Override
                                    public void run() {
                                        Toast.makeText(getApplicationContext(), "用户名或密码错误", Toast.LENGTH_LONG).show();
                                    }
                                });
                            }
                        } catch (SQLException e) {
                            e.printStackTrace();
                        }
                    }
                });
                thread.start();
            }else {
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        Toast.makeText(getApplicationContext(), "用户名或密码为空", Toast.LENGTH_LONG).show();
                    }
                });
            }

        }
        else if(view.getId()==R.id.pass_hide)
        {
            i++;
            if (i%2==0){
                pass_hide.setImageResource(R.drawable.pwd_eye);
                edt_password.setTransformationMethod(PasswordTransformationMethod.getInstance());//密码不可见
            }else {
                pass_hide.setImageResource(R.drawable.open);
                edt_password.setTransformationMethod(HideReturnsTransformationMethod.getInstance()); //密码可见
            }
        }
        else if(view.getId()==R.id.txv_zc)
        {
            Intent it = new Intent(getApplicationContext(), Register.class);
            startActivity(it);
        }
    }
}