package com.example.a123.wifictrl;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.List;

import android.database.SQLException;

public class SqlServer {
    String UserName = "";//用户名
    String Password = "";//密码
    String DataBaseName="";
    String ip="";
    Connection con = null;


    public SqlServer(String _Username, String _Password, String _DataBaseName, String _ip)
    {
        UserName =_Username;
        Password=_Password;
        DataBaseName=_DataBaseName;
        ip=_ip;
    }
    public void connect()
    {
        try { // 加载驱动程序
            Class.forName("net.sourceforge.jtds.jdbc.Driver");
            con = DriverManager.getConnection(
                    "jdbc:jtds:sqlserver://"+ip+"/"+DataBaseName, UserName,
                    Password);
        } catch (ClassNotFoundException e) {
            System.out.println("加载驱动程序出错");
        } catch (SQLException e) {
            System.out.println(e.getMessage());
        } catch (Exception e) {
            System.out.println(e.getMessage());

        }
    }


//执行SQL语句
    public void SQLstr(String sql) throws java.sql.SQLException {

        try {

            //String sql = "SELECT * FROM Ad";//查询表名为“table_test”的所有内容
            Statement stmt = con.createStatement();//创建Statement
            stmt.executeQuery(sql);//ResultSet类似Cursor
            stmt.close();
        } catch (SQLException e) {
            System.out.println(e.getMessage().toString());
        } finally {
            if (con != null)
                try {
                    con.close();
                } catch (SQLException e) {
                }
        }
    }
    //查询并返回ResultSet
    public ResultSet GetResultSet(String sql)
    {
        Statement stmt = null;
        try {
            stmt = con.createStatement();
        } catch (java.sql.SQLException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }//    Statement
        ResultSet rs = null;
        try {
            rs = stmt.executeQuery(sql);
        } catch (java.sql.SQLException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }//ResultSet    Cursor
        return rs;
    }
    // 查询并转换成集合数据
    public List<Result> getList(String sql) throws java.sql.SQLException {
        List<Result> adList = new ArrayList<Result>();
        try {
            Statement stmt = con.createStatement();//    Statement
            ResultSet rs = stmt.executeQuery(sql);//ResultSet    Cursor
            while (rs.next()) {//<code>ResultSet</code>   ָ   һ
                Result myResult = new Result(rs.getString("id"),rs.getString("number"));
                adList.add(myResult);
            }

            rs.close();
            stmt.close();
        } catch (SQLException e) {
            System.out.println(e.getMessage().toString());
        } finally {
            if (con != null)
                try {
                    con.close();
                } catch (SQLException e) {
                }
        }
        return adList;
    }

    // 返回影响条数
    public int GetNum(String sql) throws java.sql.SQLException {
        int rs = 0;
        try {
            Statement stmt = con.createStatement();//创建Statement
            rs = stmt.executeUpdate(sql);//ResultSet类似Cursor
            stmt.close();
        } catch (SQLException e) {
            System.out.println(e.getMessage().toString());
        } finally {
            if (con != null)
                try {
                    con.close();
                } catch (SQLException e) {
                }
        }
        return rs;
    }
    //查询，返回单条结果
    public String Select(String sql,String lieming) throws java.sql.SQLException {
        String reauslt="";
        try {
            Statement stmt = con.createStatement();//创建Statement
            ResultSet rs = stmt.executeQuery(sql);//ResultSet类似Cursor

            while (rs.next()) {//<code>ResultSet</code>最初指向第一行
                reauslt=rs.getString(lieming);
            }

            rs.close();
            stmt.close();
        } catch (SQLException e) {
            System.out.println(e.getMessage().toString());
        } finally {
            if (con != null)
                try {
                    con.close();
                } catch (SQLException e) {
                }
        }
        return reauslt;
    }
}
class Result {
    String data1="";
    String data2="";
    public Result(String _data1,String _data2) {
        super();
        this.data1=_data1;
        this.data2=_data1;

    }
}