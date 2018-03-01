void ssd_adc(){
  gStyle->SetPadGridX(1);
  gStyle->SetPadGridY(1);
  TCanvas *ssd=new TCanvas("ssd","adc-ssd",1200,800);
  ssd->Divide(2,2);

  /*
  for(int i=0;i<4;i++){
    ssd->cd(i+1);
    p4->Draw(Form("a1[%d]",i),Form("t0[%d]<4000",i));
   }
  */
  ssd->cd(1);
  p4->Draw("a1[0]>>h0(700,0,7)","t0[0]<4000");
  ssd->cd(2);
  p4->Draw("a1[1]>>h1(700,0,7)","t0[1]<4000");
  ssd->cd(3);
  p4->Draw("a1[2]>>h2(700,0,7)","t0[2]<4000");
  ssd->cd(4);
  p4->Draw("a1[3]>>h3(700,0,7)","t0[3]<4000");
}
