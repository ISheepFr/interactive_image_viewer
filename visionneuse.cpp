#include "visionneuse.h"

visionneuse::visionneuse(Mat flux, String directory):d_flux{flux},d_directory{directory}
{

}


double visionneuse::dist2color(Vec3b color_pixel, Vec3b spec_color)
{
    int r_diff = color_pixel[2] - spec_color[2];
    int g_diff = color_pixel[1] - spec_color[1];
    int b_diff = color_pixel[0] - spec_color[0];

    return std::sqrt(static_cast<double>(r_diff * r_diff + g_diff * g_diff + b_diff * b_diff));
}


void visionneuse::afficherEnsembleImage()
{
   
    int marge = 10;
    int x = 0 + marge;
    int y = pGauche.y + marge;
 
    for (int i = 0; i < nb_images; i++)
    {
        cv::Mat overlayImage = cv::imread(fn[i]);
        cv::resize(overlayImage, overlayImage, Size(resize_w, resize_h));

        cv::Rect roi( x, y, overlayImage.cols, overlayImage.rows);

        d_point.push_back(Point(x,y));

        // Copier l'image overlay dans la ROI du frame
        overlayImage.copyTo(d_flux(roi));
        x = x + resize_w + marge;
        
    }
    

    
    
}

void visionneuse::afficherRectBas()
{
    pGauche = Point(0, d_height - ((d_height / 3)/2));
    pDroite = Point(d_width-1, d_height-1);
    rectangle(d_flux, pGauche, pDroite, Scalar(255, 0, 0),-1);
}

void visionneuse::afficherCurrentPic()
{
    Point p1(d_point[current_pic].x - 1, d_point[current_pic].y - 1);
    Point p2(d_point[current_pic].x + resize_w+1, d_point[current_pic].y + resize_h +1);
    rectangle(d_flux, p1, p2, Scalar(0, 0, 255),2);
}

void visionneuse::afficherBoutons()
{
    // Dimensions des rectangles
    int largeurRectangle = 50;
    int hauteurRectangle = 200;
    int espacement = 10;

    Point p1_avant(espacement,d_height/2 - hauteurRectangle);
    Point p2_avant(espacement + largeurRectangle, d_height / 2);

    rectangle(d_flux, p1_avant, p2_avant, Scalar(0, 255, 0),1);


    Point p1_apres(d_width - espacement, d_height / 2);
    Point p2_apres(d_width - espacement - largeurRectangle, d_height / 2 - hauteurRectangle);

    rectangle(d_flux, p1_apres, p2_apres, Scalar(0, 0, 255), 1);

    d_basDroite_gauche = p2_avant;
    d_hautGauche_gauche = p1_avant;

    d_basDroite_droite = p1_apres;
    d_hautGauche_droite = p2_apres;

}

void visionneuse::nextImage()
{
    if (current_pic + 1 >= nb_images)
    {
        current_pic = 0;
    }
    else {
        current_pic = current_pic + 1;
    }
}

void visionneuse::precImage()
{
    if (current_pic - 1 < 0)
    {
        current_pic = nb_images - 1;
    }
    else {
        current_pic = current_pic - 1;
    }
}

void visionneuse::zoomCurrentPic()
{
    cv::Mat overlayImage = cv::imread(fn[current_pic]);

    cv::resize(overlayImage, overlayImage, Size( p2_fullscreen_basDroit.x - p1_fullscreen_gaucheHaut.x, p2_fullscreen_basDroit.y - p1_fullscreen_gaucheHaut.y ));

    cv::Rect roi(p1_fullscreen_gaucheHaut.x, p1_fullscreen_gaucheHaut.y, overlayImage.cols, overlayImage.rows);

    // Copier l'image overlay dans la ROI du frame
    overlayImage.copyTo(d_flux(roi));
}

boolean visionneuse::checkLeftButton()
{
    int count = 0;
    for (int j = d_hautGauche_gauche.y; j <= d_basDroite_gauche.y; j++)
     {
    for (int i = d_hautGauche_gauche.x; i <= d_basDroite_gauche.x; i++)
    {
        
            cv::Vec3b couleur_pixel = d_flux.at<cv::Vec3b>(j, i);
           
            if (dist2color(d_color, couleur_pixel) < 5)
            {
                count++;
            }
        }
    }
    return (count > 0);

   
}

boolean visionneuse::checkRightButton()
{
    int count = 0;
    for (int j = d_hautGauche_droite.y; j <= d_basDroite_droite.y; j++)
    {
        for (int i = d_hautGauche_droite.x; i <= d_basDroite_droite.x; i++)
        {

            cv::Vec3b couleur_pixel = d_flux.at<cv::Vec3b>(j, i);

            if (dist2color(d_color, couleur_pixel) < 5)
            {
                count++;
            }
        }
    }
    return (count > 0);

}

void visionneuse::start()
{
    if (d_cap.open(0))
    {
        cv::namedWindow("Flux original", cv::WINDOW_AUTOSIZE);
        d_width = d_cap.get(cv::CAP_PROP_FRAME_WIDTH);
        d_height = d_cap.get(cv::CAP_PROP_FRAME_HEIGHT);
        setMouseCallback("Flux original", visionneuse::onMouse, this);
    }

    glob(d_directory + "/*.jpg", fn, false);

    vector<Mat> images;
    nb_images = fn.size(); 

    while (true)
    {
        Mat eq;
        d_cap >> d_flux;
        cv::flip(d_flux, d_flux, 1);

        d_flux_green = d_flux.clone();
        d_flux_red = d_flux.clone();
        
        afficherRectBas();
        afficherEnsembleImage();
        afficherBoutons();
        afficherCurrentPic();
        
        p1_fullscreen_gaucheHaut = Point(d_basDroite_gauche.x + 20, 10);
        p2_fullscreen_basDroit = Point(d_hautGauche_droite.x - 20, d_height - ((d_height / 3) / 2) - 10);
        zoomCurrentPic();
        
        if (d_vec_color.size() == 1)
        {
            bool tempClick = checkLeftButton();
            if (tempClick == true) {
                if (last == false)
                {
                    precImage();
                    last = true;
                }
                
            }

            else {
                if (last == true)
                {
                    last = false;
                }
            }


            bool tempClickr = checkRightButton();
            if (tempClickr == true) {
                if (lastr == false)
                {
                    nextImage();
                    lastr = true;
                }

            }

            else {
                if (lastr == true)
                {
                    lastr = false;
                }
            }

        }

       

    imshow("Flux original", d_flux);

	if (waitKey(30) == 27)
		break;

}

d_cap.release();
destroyAllWindows();
}

