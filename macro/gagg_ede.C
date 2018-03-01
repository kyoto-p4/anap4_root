void gagg_ede(){
  gStyle->SetPadGridX(1);
  gStyle->SetPadGridY(1);

  p4->Draw("a2[5]:a2[3]","t0[5]<4000 && t0[3]<4000");
}
