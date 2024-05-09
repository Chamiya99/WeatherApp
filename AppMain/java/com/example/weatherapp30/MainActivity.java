package com.example.weatherapp30;

import android.os.Bundle;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;

import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

public class MainActivity extends AppCompatActivity {

    private DatabaseReference mDatabase;
    private TextView humidityTextView, temperatureTextView, pressureTextView , maxminTextView , RealFellTextView , LocationTextView ,WStaetement;

    private double fltTempMax =0.0;
    private double fltTempMin = 0.0;
    private double fltTempMid = 0.0;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Initialize Firebase database reference
        mDatabase = FirebaseDatabase.getInstance().getReference().child("DHT_11");

        // Initialize UI elements
        humidityTextView = findViewById(R.id.humidityTextView);
        temperatureTextView = findViewById(R.id.TemperatureView);
        pressureTextView = findViewById(R.id.pressureTextView);
        maxminTextView = findViewById(R.id.maxmintextview);
        RealFellTextView = findViewById(R.id.temperatureTextView);
        LocationTextView = findViewById(R.id.LocationTextView);
        WStaetement = findViewById(R.id.wstatementtextview);

        // Attach a ValueEventListener to listen for changes in data
        mDatabase.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot dataSnapshot) {
                if (dataSnapshot.exists()) {
                    // Retrieve and display data
                    Long humidityLong = dataSnapshot.child("Humidity").getValue(Long.class);
                    Long temperatureLong = dataSnapshot.child("Temperature").getValue(Long.class);
                    Long pressureLong = dataSnapshot.child("Pressure").getValue(Long.class);
                    String StringLocation = dataSnapshot.child("location").getValue(String.class);

                    if (humidityLong != null) {
                        String humidity = String.valueOf(humidityLong);
                        humidityTextView.setText(humidity + "%");
                    } else {
                        humidityTextView.setText("N/A");
                    }

                    if (temperatureLong != null) {
                        String temperature = String.valueOf(temperatureLong);
                        temperatureTextView.setText(temperature + "°C");
                    } else {
                        temperatureTextView.setText("N/A");
                    }

                    if (pressureLong != null) {
                        String pressure = String.valueOf(pressureLong);
                        pressureTextView.setText(pressure + "mbar");
                    } else {
                        pressureTextView.setText("N/A");
                    }

                    if (StringLocation != null) {
                        LocationTextView.setText(StringLocation);
                    } else {
                        LocationTextView.setText("N/A");
                    }

                    if(fltTempMin == 0.0 && fltTempMin == 0.0 ){
                        fltTempMin = temperatureLong;
                        fltTempMax = temperatureLong;
                    }else {
                        if (temperatureLong >= fltTempMax) {
                            fltTempMax = temperatureLong;
                        }
                        if (temperatureLong <= fltTempMin) {
                            fltTempMin = temperatureLong;
                        }
                    }

                    String strTempMax = String.valueOf(fltTempMax);
                    String strTempMin = String.valueOf(fltTempMin);
                    maxminTextView.setText(strTempMax + "/" + strTempMin);

                    fltTempMid = (fltTempMax + fltTempMin)/2;
                    String strTempMid = String.valueOf(fltTempMid);
                    RealFellTextView.setText(strTempMid + "°C");

                    // Generate weather statement
                    String weatherStatement = getWeatherStatement(temperatureLong, humidityLong, pressureLong);

                    // Display weather statement
                    WStaetement.setText(weatherStatement);


                }
            }

            @Override
            public void onCancelled(DatabaseError databaseError) {
                // Handle any errors
            }
        });
    }
    private String getWeatherStatement(long temperature, long humidity, long pressure) {
        // Define your conditions and thresholds here
        // You can use the provided getWeatherStatement function from the previous response

        // Example conditions
        boolean isHighTemperature = temperature > 30;
        boolean isLowHumidity = humidity < 30;
        boolean isHighPressure = pressure > 1015;

        // Generate weather statement based on conditions
        if (isHighTemperature && isLowHumidity && isHighPressure) {
            return "Hot and dry weather with high pressure.";
        } else if (isHighTemperature) {
            return "Hot weather.";
        } else if (isLowHumidity) {
            return "Dry weather.";
        } else if (isHighPressure) {
            return "High-pressure system.";
        } else {
            return "Normal weather conditions.";
        }
    }
}
