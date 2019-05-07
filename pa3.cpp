void DFTShift(Mat& I) {
	
	int cx = I.cols / 2;
	int cy = I.rows / 2;

	// Define quadrants
	Mat q0(I, Rect(0, 0, cx, cy));   // Top-Left 
	Mat q1(I, Rect(cx, 0, cx, cy));  // Top-Right
	Mat q2(I, Rect(0, cy, cx, cy));  // Bottom-Left
	Mat q3(I, Rect(cx, cy, cx, cy)); // Bottom-Right

	Mat temp;                           // swap quadrants (q0 and q2)
	q0.copyTo(temp);
	q3.copyTo(q0);
	temp.copyTo(q3);

	q1.copyTo(temp);                    // swap quadrant (q1 and q3)
	q2.copyTo(q1);
	temp.copyTo(q2);

}

void DFT(Mat Input, Mat& Real, Mat& Imag) {
	// I:    [In]   Gray Image
	// Real: [Out]  Real part of DFT
	// Imag: [Out]  Imaginary part of DFT

	// Converting input image to type float 
	Mat I, II;
	Input.convertTo(I, CV_32FC1);

	// Creating two channel input to represent
	Mat channels[] = { I, Mat::zeros(I.size(), CV_32FC1) };
	merge(channels, 2, II);

	// Calculate DFT
	dft(II, II);

	// Returning results
	split(II, channels);
	Real = channels[0];
	Imag = channels[1];
}

void IDFT(Mat& I, Mat Real, Mat Imag) {
	// I:    [Out] Gray Image
	// Real: [In]  Real part of DFT
	// Imag: [In]  Imaginary part of DFT
	
	// Merging Real and Imag
	Mat II, J;
	Mat channels[] = { Real, Imag };
	merge(channels, 2, II);

	// Calculate IDFT
	dft(II, II, DFT_INVERSE);

	// Returning results
	split(II, channels);
	magnitude(channels[0], channels[1], J);
	normalize(J, J, 0, 255, NORM_MINMAX);
	J.convertTo(I, CV_8UC1);
}

Mat NotchFilter(int s, int lowerCutOff, int upperCutOff) {
	// s: size of filter (e.g. 512 for 512x512)
	// lowerCutOff: radius of smaller circle (e.g. 40)
	// upperCutOff: radius of bigger circle (e.g. 60)

	// Sanity check
	assert(lowerCutOff < upperCutOff);

	// Creating the filter
	Mat I = Mat::ones(Size(s, s), CV_32FC1);
	int cx = I.cols / 2;
	int cy = I.rows / 2;

	// Drawing
	circle(I, Point(cx, cy), upperCutOff, 0, -1);
	circle(I, Point(cx, cy), lowerCutOff, 1, -1);

	// Return result
	return I;
}