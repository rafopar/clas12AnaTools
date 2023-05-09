g++ SkimZeroSuppression.cc -o SkimZeroSuppression.exe `root-config --cflags --libs` -L$HIPODIR/lib -lhipo4 -llz4 -I$HIPODIR/hipo4 -I./include -L./lib -luRwellTools
