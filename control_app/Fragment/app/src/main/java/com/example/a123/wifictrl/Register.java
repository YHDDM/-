package com.example.a123.wifictrl;

import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.content.Context;
import android.os.Bundle;
import android.text.InputType;
import android.text.method.HideReturnsTransformationMethod;
import android.text.method.PasswordTransformationMethod;
import android.util.Log;
import android.view.View;
import android.view.Window;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.Toast;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Register extends AppCompatActivity implements View.OnClickListener {

    ImageView img_btn;
    ImageView img_eye;
    ImageView  img_cancel;
    EditText edt_name,edt_pwd,edt_phone,edt_email,edt_again_pwd;
    @SuppressLint("MissingInflatedId")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        //隐藏标题栏，一定要在setContentView之前
        requestWindowFeature(Window.FEATURE_NO_TITLE);//remove title bar  即隐藏标题栏
        getSupportActionBar().hide();// 隐藏ActionBar
        setContentView(R.layout.activity_register);

        img_btn = findViewById(R.id.img_btn);
        img_eye = findViewById(R.id.img_eye);
        img_cancel = findViewById(R.id.img_cancel);
        edt_name = findViewById(R.id.edt_name);
        edt_pwd = findViewById(R.id.edt_pwd);
        edt_phone = findViewById(R.id.edt_phone);
        edt_email = findViewById(R.id.edt_email);
        edt_again_pwd = findViewById(R.id.edt_again_pwd);

        img_btn.setOnClickListener(this);
        img_eye.setOnClickListener(this);
        img_cancel.setOnClickListener(this);
    }
    int i = 0;
    public static Context context = null;
    public static Toast toast = null;
    public void showToast(Context context,String text) {
            if (toast == null) {
                toast = Toast.makeText(context, text, Toast.LENGTH_SHORT);
            } else {
                toast.setText(text);
                toast.setDuration(Toast.LENGTH_SHORT);
            }
            toast.show();
        }
    public static boolean isEmail(String email){
        if (null==email || "".equals(email)) return false;
        //Pattern p = Pattern.compile("\\w+@(\\w+.)+[a-z]{2,3}"); //简单匹配
        Pattern p =  Pattern.compile("\\w+([-+.]\\w+)*@\\w+([-.]\\w+)*\\.\\w+([-.]\\w+)*");//复杂匹配
        Matcher m = p.matcher(email);
        return m.matches();
    }
    @Override
    public void onClick(View view) {
        if (view.getId()==R.id.img_cancel)
        {
            finish();
        }else if(view.getId()==R.id.img_eye)
        {
            i++;
            if (i%2==0){
                img_eye.setImageResource(R.drawable.pwd_eye);
                edt_pwd.setTransformationMethod(PasswordTransformationMethod.getInstance());//密码不可见
                edt_again_pwd.setTransformationMethod(PasswordTransformationMethod.getInstance());//密码不可见
            }else {
                img_eye.setImageResource(R.drawable.open);
                edt_pwd.setTransformationMethod(HideReturnsTransformationMethod.getInstance()); //密码可见
                edt_again_pwd.setTransformationMethod(HideReturnsTransformationMethod.getInstance()); //密码可见
            }
        }
        else if(view.getId()==R.id.img_btn)
        {
            if(edt_name.getText().toString().equals("")||edt_pwd.getText().toString().equals(""))//密码和用户名为空
            {
                showToast(getApplicationContext(),"请检查填写信息");
            }else {
                if(edt_pwd.getText().toString().equals(edt_again_pwd.getText().toString())&&!edt_phone.getText().toString().equals("")&&isEmail(edt_email.getText().toString().trim()) && edt_email.getText().toString().trim().length()<=31) {
                    showToast(getApplicationContext(), "注册成功");
                    Thread thread = new Thread(new Runnable() {
                        @Override
                        public void run() {
                            SqlServer sql = new SqlServer("test", "Abc123456", "ShanDongZhao", "msg.comingthings.com:1433");
                            sql.connect();
                            //返回影响行数
                            try {
                                int a = sql.GetNum(String.format("INSERT INTO [User] VALUES ('%s','%s','%s','%s')", edt_name.getText().toString(), edt_pwd.getText().toString(), edt_phone.getText().toString(), edt_email.getText().toString()));
                                if (a == 1) {
                                    //插入成功
                                    showToast(getApplicationContext(), "注册成功");
                                } else {
                                    //插入失败
                                    showToast(getApplicationContext(), "注册失败");
                                }
                                Log.d("debug", String.valueOf(a));
                            } catch (SQLException e) {
                                e.printStackTrace();
                            }
                        }
                    });
                    thread.start();
                }

                else if(!edt_pwd.getText().toString().equals(edt_again_pwd.getText().toString())){
                    showToast(getApplicationContext(), "两次输入的密码不一致");
                } else if(!isEmail(edt_email.getText().toString().trim()) && edt_email.getText().toString().trim().length()<=31) {
                    showToast(getApplicationContext(),"邮箱格式错误");
                }
            }
        }
    }
}