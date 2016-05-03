package com.nextgis.gisrenderer;

import android.graphics.Bitmap;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;
import com.nextgis.gismaplib.GmCore;

import java.io.IOException;


public class MainActivity
    extends AppCompatActivity
{

  @Override
  protected void onCreate(Bundle savedInstanceState)
  {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.activity_main);

    final MainApplication app = (MainApplication) getApplication();
    final GmCore gmCore = app.getGmCore();


    final TextView textView = (TextView) findViewById(R.id.test_text);
    final ImageView imageView = (ImageView) findViewById(R.id.test_image);
    Button button = (Button) findViewById(R.id.test_button);
    button.setOnClickListener(new View.OnClickListener()
    {
      @Override
      public void onClick(View v)
      {
// for test
//        textView.setText(getTestText(gmCore));

        Bitmap bitmap = Bitmap.createBitmap(400, 400, Bitmap.Config.ARGB_8888);

        Boolean isImageStored = putMapboxImageData(gmCore, bitmap);

        if (null == isImageStored || !isImageStored) {
          textView.setText("...???...");
          return;
        }

        textView.setText("...IMAGE...");
        imageView.setImageBitmap(bitmap);
      }
    });
  }


  public Boolean putMapboxImageData(
      GmCore gmCore,
      Bitmap dstBitmap)
  {
    try {
      return (gmCore.putMapboxImageData(dstBitmap));
    } catch (IOException e) {
      e.printStackTrace();
      return null;
    }
  }


  public String getTestText(GmCore gmCore)
  {
    String testPath = "test";
    try {
      return (gmCore.getTestText(testPath));
    } catch (IOException e) {
      e.printStackTrace();
      return null;
    }
  }
}
