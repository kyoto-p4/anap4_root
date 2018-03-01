void gagg_adc(){
  gStyle->SetPadGridX(1);
  gStyle->SetPadGridY(1);
  TCanvas *gagg=new TCanvas("gagg","GAGG CsI ADC",400,800);
  gagg->Divide(1,2);
 
  /*
  for(int i=5;i<7;i++){
    gagg->cd(i-4);
    p4->Draw(Form("a1[%d]",i),Form("t0[%d]<4000",i));
  }
  */
  gagg->cd(1);
  p4->Draw("a1[5]>>h5(2100,0,70)","t0[5]<4000");
  gagg->cd(2);
  p4->Draw("a1[6]>>h6(2100,0,70)","t0[6]<4000");
}
