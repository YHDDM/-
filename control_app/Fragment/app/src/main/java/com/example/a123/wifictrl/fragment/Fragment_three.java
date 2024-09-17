


package com.example.a123.wifictrl.fragment;


import static androidx.core.content.ContextCompat.getSystemService;

import android.Manifest;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.PackageManager;
import android.net.wifi.ScanResult;
import android.net.wifi.WifiConfiguration;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Bundle;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;
import androidx.fragment.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.Toast;

import com.example.a123.wifictrl.R;

import java.util.List;


/**
 * A simple {@link Fragment} subclass.
 */
public class Fragment_three extends Fragment {


    private ListView listView;
    private static final int PERMISSION_REQUEST_CODE = 1;
    private WifiManager wifiManager;
    private ArrayAdapter<String> adapter;
    private List<ScanResult> wifiList;
    private View rootView;


    public Fragment_three() {
        // Required empty public constructor

    }


    private Context mContext;

    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        if (rootView == null) {
            rootView = inflater.inflate(R.layout.fragment_fragment_three, container, false);
            initView();

        } else {
            ViewGroup parent = (ViewGroup) rootView.getParent();
            if (parent != null) {
                parent.removeView(rootView);
            }
        }
        IntentFilter intentFilter = new IntentFilter();
        intentFilter.addAction(WifiManager.SCAN_RESULTS_AVAILABLE_ACTION);
        getActivity().registerReceiver(wifiReceiver,intentFilter);
        listView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                String ssid = wifiList.get(position).SSID;
                connectToWifi(ssid);
            }
        });
        return rootView;
    }

    private void initView() {
        //btn_kd= (Button) rootView.findViewById(R.id.btn_kd);
        this.mContext = getActivity();
        listView = rootView.findViewById(R.id.listView);

        adapter = new ArrayAdapter<>(getContext(), android.R.layout.simple_list_item_1);
        listView.setAdapter(adapter);
        wifiManager = (WifiManager) requireContext().getApplicationContext().getSystemService(requireContext().WIFI_SERVICE);
        rootView.findViewById(R.id.btnScanWifi).setOnClickListener(view -> {
            if (ContextCompat.checkSelfPermission(requireContext(), Manifest.permission.ACCESS_WIFI_STATE)
                    != PackageManager.PERMISSION_GRANTED || ContextCompat.checkSelfPermission(requireContext(), Manifest.permission.ACCESS_COARSE_LOCATION) !=
                    PackageManager.PERMISSION_GRANTED) {
                requestPermissions(new String[]{Manifest.permission.ACCESS_WIFI_STATE, Manifest.permission.ACCESS_COARSE_LOCATION}, PERMISSION_REQUEST_CODE);
            } else {
                scanWifi();
            }
        });
    }
    private void scanWifi() {
        if (wifiManager.isWifiEnabled()) {
            wifiManager.startScan();
            List<ScanResult> results = wifiManager.getScanResults();
            displayWifiList(results);
        } else {
            Toast.makeText(getContext(), "WiFi is disabled. Please enable WiFi.", Toast.LENGTH_SHORT).show();
        }
    }
    private void displayWifiList(List<ScanResult> results) {
        adapter.clear();
        for (ScanResult result : results) {
            String wifiDetails = result.SSID + " - " + result.level + "dBm";
            adapter.add(wifiDetails);
        }
    }
    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if (requestCode == PERMISSION_REQUEST_CODE) {
            if (grantResults.length > 1 &&
                    grantResults[0] == PackageManager.PERMISSION_GRANTED &&
                    grantResults[1] == PackageManager.PERMISSION_GRANTED) {
                scanWifi();
            } else {
                Toast.makeText(getContext(), "Permissions are required to scan WiFi networks.", Toast.LENGTH_SHORT).show();
            }
        }
    }
    private void connectToWifi(String networkSSID) {
        WifiConfiguration conf = new WifiConfiguration();
        conf.SSID = "\"" + networkSSID + "\"";
        conf.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.NONE); // Open network

        int networkId = wifiManager.addNetwork(conf);
        wifiManager.disconnect();
        wifiManager.enableNetwork(networkId, true);
        wifiManager.reconnect();
    }
    private final BroadcastReceiver wifiReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            wifiList = wifiManager.getScanResults();
            String[] wifiNames = new String[wifiList.size()];
            for (int i = 0; i < wifiList.size(); i++) {
                wifiNames[i] = wifiList.get(i).SSID;
            }

            ArrayAdapter<String> adapter = new ArrayAdapter<>(getContext(), android.R.layout.simple_list_item_1, wifiNames);
            listView.setAdapter(adapter);
        }
    };
    @Override
    public void onDestroy() {
        super.onDestroy();
        getActivity().unregisterReceiver(wifiReceiver);
    }


}