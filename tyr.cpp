// ################################################################################
// #                                                                              #
// # tyr                                                                          #
// #                                                                              #
// # Will Breaden Madden                                                          #
// #                                                                              #
// ################################################################################
// #                                                                              #
// # DESCRIPTION                                                                  #
// #                                                                              #
// # This program can be used to create ROOT graphs using CSV data. Command line  #
// # options can be used to specify file names, axis limits, titles, file output  #
// # formats and terminal output verbosity.                                       #
// #                                                                              #
// # COMMAND LINE OPTIONS                                                         #
// #                                                                              #
// #     -i    input file name              (required)                            #
// #     -o    output file name             (required)                            #
// #     -t    graph title                                                        #
// #     -x    graph x axis title                                                 #
// #     -y    graph y axis title                                                 #
// #     -a    y axis maximum                                                     #
// #     -b    y axis minimum                                                     #
// #     -c    x axis maximum                                                     #
// #     -d    x axis minimum                                                     #
// #     -v    verbose flag                                                       #
// #     -f    output file all formats flag                                       #
// #                                                                              #
// # EXAMPLES                                                                     #
// #                                                                              #
// # converting data to graph with verbosity                                      #
// #                                                                              #
// #     tyr -i data.txt -o data.eps -v                                           #
// #                                                                              #
// # converting data to graph with verbosity, outputting in all possible formats  #
// #                                                                              #
// #     tyr -i data.txt -o data -f -v                                            #
// #                                                                              #
// # converting data input using Bash process substitution to graph               #
// #                                                                              #
// #     tyr -i <(echo "${dataInCSVFormat}") -o graph.eps                         #
// #                                                                              #
// # converting data to graph, defining titles                                    #
// #                                                                              #
// #     tyr -i data.txt -o graph.eps \                                           #
// #         -t "training test error versus epochs" \                             #
// #         -x "epochs" \                                                        #
// #         -y "error measure"                                                   #
// #                                                                              #
// # converting data to graph, defining titles and axes limits                    #
// #                                                                              #
// #     tyr -i data.txt -o graph.eps \                                           #
// #         -t "training test error versus epochs" \                             #
// #         -x "epochs" \                                                        #
// #         -y "error measure" \                                                 #
// #         -a 70 \                                                              #
// #         -b 50 \                                                              #
// #         -c 22 \                                                              #
// #         -d 7                                                                 #
// #                                                                              #
// ################################################################################
// #                                                                              #
// # LICENCE INFORMATION                                                          #
// #                                                                              #
// # This program provides graphing capabilities.                                 #
// #                                                                              #
// # copyright (C) 2013 2014 William Breaden Madden                               #
// #                                                                              #
// # This software is released under the terms of the GNU General Public License  #
// # version 3 (GPLv3).                                                           #
// #                                                                              #
// # This program is free software: you can redistribute it and/or modify it      #
// # under the terms of the GNU General Public License as published by the Free   #
// # Software Foundation, either version 3 of the License, or (at your option)    #
// # any later version.                                                           #
// #                                                                              #
// # This program is distributed in the hope that it will be useful, but WITHOUT  #
// # ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or        #
// # FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for    #
// # more details.                                                                #
// #                                                                              #
// # For a copy of the GNU General Public License, see                            #
// # <http://www.gnu.org/licenses/>.                                              #
// #                                                                              #
// ################################################################################

// standard libraries
    // definition of standard input/output stream objects
        #include <iostream>
    // file input and output operations
        #include <fstream>
    // manipulate strings as though they were input/output streams
    // (handle conversion from char* to double)
        #include <sstream>
    // input and output operations
        #include <stdio.h>
    // manipulate C strings and arrays
        #include <string.h>
    // mathematics
        #include <cmath>
    // getopts (handle command line options and arguments)
        #include <unistd.h>
    // Standard General Utilities Library
        #include <stdlib.h>
    // classify and transform individual characters
        #include <ctype.h>
// ROOT libraries:
    #include "TStyle.h"
    #include "TROOT.h"
    #include "TPluginManager.h"
    #include "TSystem.h"
    #include "TFile.h"
    #include "TGaxis.h"
    #include "TCanvas.h"
    #include "TH1.h"
    #include "TF1.h"
    #include "TLatex.h"
    #include "TLine.h"
    #include "TSpline.h"
    #include "TGraph.h"
    #include "TStopwatch.h"
    #include "TError.h"
    #include "TChain.h"
    // RooFit libraries:
        //#include "RooAbsData.h"
        //#include "RooDataHist.h"
        //#include "RooCategory.h"
        //#include "RooDataSet.h"
        //#include "RooAbsPdf.h"
        //#include "RooSimultaneous.h"
        //#include "RooProdPdf.h"
        //#include "RooNLLVar.h"
        //#include "RooProfileLL.h"
        //#include "RooFitResult.h"
        //#include "RooPlot.h"
        //#include "RooRandom.h"
        //#include "RooMinuit.h"
        //#include "TRandom3.h"
        //#include "RooWorkspace.h"
        //#include "RooStats/RooStatsUtils.h"
        //#include "RooStats/ModelConfig.h"
using namespace std;
//using namespace RooFit;
//using namespace RooStats;

double returnDoubleFromPointerToChar(const char *cText){
    std::stringstream ss ( cText );
    double dText = 0;
    ss >> dText;
    return dText;
}

int main (int argc, char **argv){
    // disable ROOT messages
    gErrorIgnoreLevel = 5000;
    string fileName1 = "";                  // input file name              (i) (required input)
    string fileName2 = "";                  // output file name             (o) (required input)
    string graphTitleMain = "";             // graph title                  (t)
    string graphTitleAxisx = "";            // graph x axis title           (x)
    string graphTitleAxisy = "";            // graph y axis title           (y)
    double axisyMaximum = DBL_MAX;          // y axis maximum               (a)
    double axisyMinimum = DBL_MAX;          // y axis minimum               (b)
    double axisxMaximum = DBL_MAX;          // x axis maximum               (c)
    double axisxMinimum = DBL_MAX;          // x axis minimum               (d)
    int verboseFlag = 0;                    // verbose flag                 (v)
    int outputAllFormatsFlag1 = 0;          // output file all formats flag (f)
    int index;                              // internal variable
    int c;                                  // internal variable
    string fileNameExtension1;              // internal variable
    string fileName2WithFileNameExtension1; // internal variable
    opterr = 0;

    // process command line arguments and options
    while ((c = getopt (argc, argv, "i:o:t:x:y:a:b:c:d:fv")) != -1)
        switch (c){
            case 'i':
                fileName1 = optarg;
                break;
            case 'o':
                fileName2 = optarg;
                break;
            case 't':
                graphTitleMain = optarg;
                break;
            case 'x':
                graphTitleAxisx = optarg;
                break;
            case 'y':
                graphTitleAxisy = optarg;
                break;
            case 'a':
                axisyMaximum = returnDoubleFromPointerToChar(optarg);
                break;
            case 'b':
                axisyMinimum = returnDoubleFromPointerToChar(optarg);
                break;
            case 'c':
                axisxMaximum = returnDoubleFromPointerToChar(optarg);
                break;
            case 'd':
                axisxMinimum = returnDoubleFromPointerToChar(optarg);
                break;
            case 'f':
                outputAllFormatsFlag1 = 1;
                break;
            case 'v':
                verboseFlag = 1;
                break;
            case '?':
                if (
                    optopt == 'i' ||
                    optopt == 'o' ||
                    optopt == 't' ||
                    optopt == 'x' ||
                    optopt == 'y' ||
                    optopt == 'a' ||
                    optopt == 'b' ||
                    optopt == 'c' ||
                    optopt == 'd'
                ){
                    fprintf (stderr, "option -%c requires argument\n", optopt);
                }
                else if (isprint (optopt)){
                    fprintf (stderr, "unknown option -%c\n", optopt);
                }
                else {
                    fprintf (stderr, "unknown option character '\\x%x'\n", optopt);
                }
		return 1;
                default:
                abort();
        }
        for (index = optind; index < argc; index++) printf ("non-option argument %s\n", argv[index]);
        if (verboseFlag == 1){
            cout << endl;
            cout << "input file name: " << fileName1 << endl;
            cout << "output file name: " << fileName2 << endl;
        }
    // x variable
    vector<double> x;
    // y variable
    vector<double> y;
    // Access the data in the input file.
    ifstream file1 (fileName1.c_str());
    string line;
    int currentLineNumber = 0;
    if (verboseFlag == 1) {cout << "reading data from file " << fileName1 << "..." << endl;}
    while (getline (file1, line)){
        currentLineNumber++;
        if (verboseFlag == 1) {cout << "line " << currentLineNumber << ": ";}
        istringstream linestream(line);
        string item;
        int itemNumber = 0;
        while (getline (linestream, item, ',')){
            itemNumber++;
            if (verboseFlag == 1) {cout << "item " << itemNumber << ": " << item << " ";}
            // Store x datum.
            if (itemNumber == 1) {x.push_back(atof(item.c_str()));}
            // Store y datum.
            if (itemNumber == 2) {y.push_back(atof(item.c_str()));}
        }
        if (verboseFlag == 1) {cout << endl;}
    }		
    file1.close();
    int numberOfLinesInInputFile = currentLineNumber + 1;
    // number of data points
    int n = numberOfLinesInInputFile - 1;
    // Create a new canvas.
    TCanvas *canvas1 = new TCanvas(graphTitleMain.c_str(), graphTitleMain.c_str());
    //canvas1->SetLogy();
    // Create a new graph.
    TGraph *graph1 = new TGraph(n, &x[0], &y[0]);
    // Set the graph titles.
    if (verboseFlag == 1){
        cout << "graph main title: " << graphTitleMain << endl;
        cout << "graph x axis title: " << graphTitleAxisx << endl;
        cout << "graph y axis title: " << graphTitleAxisy << endl;
    }
    graph1->SetTitle(graphTitleMain.c_str());
    graph1->GetXaxis()->SetTitle(graphTitleAxisx.c_str());
    graph1->GetYaxis()->SetTitle(graphTitleAxisy.c_str());
    // Set the marker styles.
    graph1->SetMarkerColor(2); // red
    graph1->SetMarkerStyle(kFullCircle); // circle
    graph1->SetMarkerSize(1); // default size
    // Set the graph range, if ranges have been specified.
    if (
        axisyMaximum != DBL_MAX &&
        axisyMinimum != DBL_MAX
    ){
        if (verboseFlag == 1){
            cout << "graph y axis minimum: " << axisyMinimum << endl;
            cout << "graph y axis maximum: " << axisyMaximum << endl;
        }
        graph1->GetYaxis()->SetRangeUser(axisyMinimum, axisyMaximum);
    }
    if (
        axisxMaximum != DBL_MAX &&
        axisxMinimum != DBL_MAX
    ){
        if (verboseFlag == 1){
            cout << "graph x axis minimum: " << axisxMinimum << endl;
            cout << "graph x axis maximum: " << axisxMaximum << endl;
        }
        graph1->GetXaxis()->SetRangeUser(axisxMinimum, axisxMaximum);
    }
    // Draw the canvas, then draw the graph.
    canvas1->Draw();
    graph1->Draw("ALP");
    //graph1->Draw("AP");
    // Save the canvas to an image file or to image files, as specified.
    if (outputAllFormatsFlag1 == 0){
        if (verboseFlag == 1) {cout << "saving file " << fileName2 << "..." << endl;}
        canvas1->SaveAs(fileName2.c_str());
    }
    if (outputAllFormatsFlag1 == 1){
        // Save to EPS format.
        fileNameExtension1 = ".eps";
        fileName2WithFileNameExtension1 = fileName2 + fileNameExtension1;
        if (verboseFlag == 1) {cout << "saving file " << fileName2WithFileNameExtension1 << "..." << endl;}
        canvas1->SaveAs(fileName2WithFileNameExtension1.c_str());
        // Save to PDF format.
        fileNameExtension1 = ".pdf";
        fileName2WithFileNameExtension1 = fileName2 + fileNameExtension1;
        if (verboseFlag == 1) {cout << "saving file " << fileName2WithFileNameExtension1 << "..." << endl;}
        canvas1->SaveAs(fileName2WithFileNameExtension1.c_str());
        // Save to GIF format.
        fileNameExtension1 = ".gif";
        fileName2WithFileNameExtension1 = fileName2 + fileNameExtension1;
        if (verboseFlag == 1) {cout << "saving file " << fileName2WithFileNameExtension1 << "..." << endl;}
        canvas1->SaveAs(fileName2WithFileNameExtension1.c_str());
        // Save to JPG format.
        fileNameExtension1 = ".jpg";
        fileName2WithFileNameExtension1 = fileName2 + fileNameExtension1;
        if (verboseFlag == 1) {cout << "saving file " << fileName2WithFileNameExtension1 << "..." << endl;}
        canvas1->SaveAs(fileName2WithFileNameExtension1.c_str());
        // Save to C++ format.
        fileNameExtension1 = ".cxx";
        fileName2WithFileNameExtension1 = fileName2 + fileNameExtension1;
        if (verboseFlag == 1) {cout << "saving file " << fileName2WithFileNameExtension1 << "..." << endl;}
        canvas1->SaveAs(fileName2WithFileNameExtension1.c_str());
    }
    if (verboseFlag == 1) {cout << endl;}
    return 0;
}