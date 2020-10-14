package com.example.firebase;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.os.Handler;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;

import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

public class MainActivity extends AppCompatActivity {
    TextView textview;
    Button btna, btnb;
    ImageView img;

    DatabaseReference mRootRef = FirebaseDatabase.getInstance().getReference();
    DatabaseReference conditionRef = mRootRef.child("door");
//    DatabaseReference conditionRef1 = mRootRef.child("lock");
//    DatabaseReference conditionRef2 = mRootRef.child("server");
//    DatabaseReference conditionRef3 = mRootRef.child("camera");
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        textview = (TextView) findViewById(R.id.textview);
        btna = (Button) findViewById(R.id.btna);
        btnb = (Button) findViewById(R.id.btnb);
        img = findViewById(R.id.img);
    }
    @Override
    protected void onStart() {
        super.onStart();
        conditionRef.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot dataSnapshot) {
                String text = dataSnapshot.getValue(String.class);
                textview.setText("Status: "+text);
                if(text.equals("a")){
                    img.setImageResource(R.mipmap.ledon);
                }
                else if(text.equals("b")){
                    img.setImageResource(R.mipmap.ledon);
                }
                else{
                    img.setImageResource(R.mipmap.ledoff);
                }
            }
            @Override
            public void onCancelled(DatabaseError databaseError) {
            }
        });
        btna.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String status = "a";
                conditionRef.setValue(status);
                new Handler().postDelayed(new Runnable()
                {
                    @Override
                    public void run()
                    {
                        String status = "c";
                        conditionRef.setValue(status);
                    }
                }, 3000);// 3초 정도 딜레이를 준 후 시작
            }
        });
        btnb.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String status = "b";
                conditionRef.setValue(status);
                new Handler().postDelayed(new Runnable()
                {
                    @Override
                    public void run()
                    {
                        String status = "c";
                        conditionRef.setValue(status);
                    }
                }, 3000);// 3초 정도 딜레이를 준 후 시작
            }
        });
    }
}
