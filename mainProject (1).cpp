#include <string>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <stdio.h>
#include <fstream>
#include <algorithm> 
#include <array>
#include <gtk/gtk.h>
#include <unistd.h>


using namespace std;
using namespace cv;
bool bt_pr=false;

static void button_effect(GtkWidget,gpointer data){
bt_pr = true;
}

int gui_button()
{
//gtk_init(&argc,&argv);
GtkWidget *window, *button;
window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
button=gtk_button_new_with_label("📷");
g_signal_connect(window,"delete_event",G_CALLBACK(gtk_main_quit),NULL);
g_signal_connect(button,"clicked",G_CALLBACK(button_effect),NULL);
gtk_container_add(GTK_CONTAINER(window),button);
gtk_widget_set_size_request(window,100,100);
gtk_widget_set_size_request(button,100,100);

gtk_widget_show_all(window);
gtk_main();
return 0;

}


bool compare_wth_dic(bool debug, string words) {
		string dosage_dic[] = { "ONE","ONCE","DAILY","TWICE","ONE","DAY","MORNING","NIGHT","BED","SLEEP","EVENING","WEEKLY","BY", "MOUTH","TAKE","USE","CAPSULE","PILL","DISSOLVE","FOOD","MONTHLY","EVERY","HR","HRS","HOUR","HOURS","PRM","PER","DOSE","WATER" };
		bool found = false;
		for (int c = 0; c < ((sizeof(dosage_dic) / sizeof(*dosage_dic)) - 1); c++)
		{
			if ((words.find(dosage_dic[c])) != -1)
			{
				found = true;
				if (debug == true)
					cout << "Found a match" << endl;
			}
		}
		if (found == false)
			if (debug == true)
				cout << "No Match Found" << endl;

		return found;
	}
	bool compare_wth_drugdic(bool debug , string words) {
		string drug_names[] = { "ATORVASTATIN CALCIUM", "LIPITOR", "LEVOTHYROXINE", "SYNTHROID", "LISINOPRIL", "PRINIVIL", "OMEPRAZOLE", "PRILOSEC", "METFORMIN", "GLUCOPHAGE", "AMLODIPINE", "NORVASC", "SIMVASTATIN", "ZOCOR", "HYDROCODONE", "ACETAMINOPHEN", "LORTAB", "METOPROLOL ER", "TOPROL XL", "LOSARTAN", "COZAAR", "AZITHROMYCIN", "ZITHROMAX", "ZOLPIDEM", "AMBIEN", "HYDROCHLOROTHIAZIDE", "MICROZIDE", "FUROSEMIDE", "LASIX", "METOPROLOL", "LOPRESSOR", "PANTOPRAZOLE", "PROTONIX", "GABAPENTIN", "NEURONTIN", "AMOXICILLIN", "AMOXIL", "PREDNISONE", "DELTASONE", "SERTRALINE", "ZOLOFT", "TAMSULOSIN", "FLOMAX", "FLUTICASONE", "FLONASE", "PRAVASTATIN", "PRAVACHOL", "TRAMADOL", "ULTRAM", "MONTELUKAST", "SINGULAIR", "ESCITALOPRAM", "LEXAPRO", "CARVEDILOL", "COREG", "ALPRAZOLAM", "XANAX", "WARFARIN", "COUMADIN", "MELOXICAM", "MOBIC", "CLOPIDOGREL", "PLAVIX", "POTASSIUM CLAVULANATE ER", "AUGMENTIN XR", "ALLOPURINOL", "ZYLOPRIM", "BUPROPION", "WELLBUTRIN", "LISINOPRIL", "HCTZ", "ZESTORETIC", "CITALOPRAM", "CELEXA", "LOSARTAN POTASSIUM", "COZAAR", "ATENOLOL", "TENORMIN", "CIALIS", "DULOXETINE", "CYMBALTA", "FLUOXETINE", "PROZAC", "FENOFIBRATE", "TRICOR", "CRESTOR", "VENLAFAXINE", "EFFEXOR", "VENTOLIN", "AMPHETAMINE", "DEXTROAMPHETAMINE", "ADDERALL", "CYCLOBENZAPRINE", "FLEXERIL", "TRAZODONE", "OLEPTRO", "METHYLPREDNISOLONE", "MEDROL", "POTASSIUM CHLORIDE", "KLOR-CON", "PROMETHAZINE" };
		bool found = false;
		for (int c = 0; c < ((sizeof(drug_names) / sizeof(*drug_names)) - 1); c++)
		{
			if ((words.find(drug_names[c])) != -1)
			{
				found = true;
				if (debug == true)
					cout << "Found a match" << endl;
			}
		}
		if (found == false)
			if (debug == true)
				cout << "No Match Found" << endl;

		return found;
	}


	int total_wrds_inline(bool debug, string linef) { //runs to count the max number of lines
		int words = 0;
		if (debug == true)
			cout << endl << linef << endl;
		long int end, begin;
		do {
			begin = linef.find_first_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
			end = linef.find_first_of(" \t", begin);
			if (begin < 0)  //handles an exception related to an empty line
				break;
			string actualword(linef, begin, end - begin);
			linef = linef.substr(end + 1);
			/*if (debug == true)
				//printf("%S \n", actualword.c_str());
*/
			words++;
		} while (end > 0);
		if (debug == true)
			cout << "The words contained in this line is " << words << endl;
		
		return words;
	}
	
	int Prescription_Read(bool debug, string PATH) {
		//PATH= "C:\\Users\\Linkle\\Desktop\\label.txt";
		//debug=false;
		long int end;
		int mlslots=0, maxwords=0, len, mwordslots=0; //used for counting
		string word; //the word being held here
		string *pwords; //used to count the words in a string
		string linetxt;
		string first;
		string::size_type charbuffer;
		int L = -1, C = -1;
		ifstream conv_txt(PATH);
		if (conv_txt.is_open()) {
			cout << "Opened File Successfully\n" << endl;
			while (getline(conv_txt, linetxt)) {
				mwordslots = total_wrds_inline(debug,linetxt);
				if (mwordslots > maxwords)
					maxwords = mwordslots;
				if (debug == true)
					cout << "Seems like the max is " << mwordslots << endl;
				mlslots++;
			}


			//2d dynamic memory array
			string** ptr = new string*[mlslots];
			for (int i = 0; i < mlslots; ++i)
				ptr[i] = new string[maxwords];



			int r = 0, c = 0;
			ifstream conv_txt(PATH);
			while (getline(conv_txt, linetxt)) {
				long int end, begin;
				bool line_complete = false;
				c = 0;

				do {

					begin = linetxt.find_first_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");//finds the position where the word starts
					end = linetxt.find_first_of(" \t\r", begin);//find the position where the word ends

					if (begin >= 0 && line_complete == false) { //handles an exception related to an empty line
						string actualword(linetxt, begin, end - begin); //Gets the seperated word from the line
						linetxt = linetxt.substr(end + 1); //purges the seperated word from the line

						/*//this part is tricjy because what it does is filter our the "\0" from every word
						for (int d = 0; d < (actualword.length() - 1); d++) {
							int clearb = d + 1;
							actualword.erase(clearb, 1);

						}*/

						ptr[r][c] = actualword;
					}

					else
						ptr[r][c] = "";

					if (end < 0)
						line_complete = true;

					c++;
				} while (c < maxwords);
				r++;
			}
			conv_txt.close();
			//cout<<"organizing words in dynamic array"<<endl;
			for (int r = 0; r < mlslots; r++) {

				for (int c = 0; c < maxwords; c++) {
					
					if (debug == true)
						cout << ptr[r][c] << endl;
					//converts all the characters to uppercase to eliminate case sensitivity 
					std::transform(ptr[r][c].begin(), ptr[r][c].end(), ptr[r][c].begin(), ::toupper);

				}
			}//this part of the code should be simplified
			string * which_line = new string[mlslots]; //creates a one dimensional array to mark a line thats of instrest
			int mostlikly = 0, finalline = 0;
			for (int r = 0; r < mlslots; r++) {
				int similar_count = 0;
				for (int c = 0; c < maxwords; c++) {//the for loop here and the one before this are used to go through the 2 dimensional lists of dynamic memory
					if (compare_wth_dic(debug,ptr[r][c])) //uses a compare function to compare each word given with the ones in the dictionary
						similar_count++;

					if (compare_wth_drugdic(debug,ptr[r][c])) {
						L = r;
						C = c;
					}
				}
				which_line[r] = similar_count; //assigns a value to an dynamic array which keeps track of which kine of text has the most similar text of the dictionary
				if (mostlikly < similar_count)
				{
					mostlikly = similar_count;
					finalline = r;
				}
			}
			cout << "Output: " << endl;
			for (int done = 0; done <= (maxwords - 1); done++)
			{
				cout << ptr[finalline][done] << " ";
			}
			cout << endl;

			/*if (L == -1 || C == -1)
				cout << "Failed to find the capsule substance within the label" << endl;
			else
				cout << "The Likely substance inside this container is " << ptr[L][C] << "." << endl;*/


		}
		else
		{
			cout << "Cannot open file (I think you broke something -__-)\n" << endl;
			return -1;
		}
	
		return 0;
	}


int Preview_N_Take() {


VideoCapture stream(0);   //0 is the id of video device.0 if you have only one camera.
stream.set(CV_CAP_PROP_FRAME_WIDTH,1280);
stream.set(CV_CAP_PROP_FRAME_HEIGHT,720);

if (!stream.isOpened()) { //check if video device has been initialised
cout << "Camera can't be found"<<endl;
return -2;
}

Mat LastFrame;

//gui_button();

while (true) {
stream.read(LastFrame);
namedWindow("Camera Preview",WND_PROP_FULLSCREEN);
setWindowProperty("Camera Preview",WND_PROP_FULLSCREEN,WINDOW_FULLSCREEN);
imshow("Camera Preview", LastFrame);
if ((waitKey(16)==32)||bt_pr==true)
{
sleep(3);
imwrite("PictureTaken.jpg",LastFrame);
//cout<<"Picture was taken"<<endl;
break;
}
}
return 0;
}



int main(int argc,char* argv[]) {
   tesseract::TessBaseAPI *ocr = new tesseract::TessBaseAPI();
   
   if (Preview_N_Take()==0)
   {
   printf("Image captured \n");
   printf("Image Saved \n");
   }
   else 
   printf("An error has ocurred when taking the picture!\n");
   

   // tesseract code to extract text from image. 
   ocr->Init(NULL, "eng");
   Pix *image = pixRead("PictureTaken.jpg");
   ocr->SetImage(image);
   char* outText;

   outText = ocr->GetUTF8Text();
   printf("OCR output:\n%s", outText);

   printf("Saving Converted Text\n");
   ofstream tofile("tess_conv_txt.txt");
   tofile << outText<<endl;
   tofile.close();
   
   ocr->End();
   delete [] outText;
   pixDestroy(&image);
   
   printf("Processing the Image to extract the prescription\n");
   Prescription_Read(true,"tess_conv_txt.txt");
   return 0;
}



