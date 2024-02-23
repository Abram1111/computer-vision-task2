import streamlit as st
from PIL import Image
from pathlib import Path
import streamlit_nested_layout
import cv2
import matplotlib.pyplot as plt
import Filters
import numpy as np
import Histograms as Hs
import Frequency as freq
from active_contour import *

images_folder = 'images'
freq_pic1 = ''
freq_pic2 = ''
st.set_page_config(layout="wide")
# with open('../style.css') as f:
    
#     st.markdown(f'<style>{f.read()}</style>', unsafe_allow_html=True)

tab1, tab2, tab3 ,tab4 = st.tabs(["Filters", "Histograms", "Frequency","active contour"])

with tab1:
    st.header("Filters")
    before_col, after_col = tab1.columns([1, 2])
    before_col2, after_col2 = tab1.columns(2)
    with before_col:
        file = st.file_uploader("Choose Image")
    with after_col:

        type, list, slider = st.columns(3)
        with type:
            opration_type = st.radio(
                "Choose opration", ["Add Noise", "Filter", "Edge Detection"], label_visibility="visible")
        with list:
            ############################## Noise Section ################################
            if opration_type == "Add Noise":
                noise_type = st.radio(
                    "Choose Noise", ["Salt&Papper", "Gaussian"])
                if noise_type == "Salt&Papper":
                    with slider:
                        noise_ratio = st.slider("Noise Ratio", min_value=0, max_value=100,
                                                value=5, step=1,  label_visibility="visible")
                if noise_type == "Gaussian":
                    with slider:
                        gaussuan_sigma = st.slider("Sigma", min_value=0.0, max_value=1.0,
                                                   value=.3, step=.1,  label_visibility="visible")
            ############################## Filter Section ################################
            elif opration_type == "Filter":
                filter_type = st.radio(
                    "Choose filter", ["Avrage Filter", "Gaussian Filter", "Median Filter"])
                with slider:
                    kernal_dim = st.slider("kernal dimention", min_value=2, max_value=10,
                                           value=3, step=1,  label_visibility="visible")
                if filter_type == "Gaussian Filter":
                    with slider:
                        gaussuan_sigma = st.slider("Sigma", min_value=0.0, max_value=1.0,
                                                   value=.3, step=.1,  label_visibility="visible")

            ############################## Edge Section ################################
            elif opration_type == "Edge Detection":
                edge_type = st.radio(
                    "Choose Operator", ["Prewitt Operator", "Sobel Operator", "Roberts Operator", "Canny Operator"])
                if edge_type == "Sobel Operator":
                    with slider:
                        x_dir = st.checkbox("Soble x", False)
                        y_dir = st.checkbox("Soble y", False)

    if file is not None:
        name = 'images\\'+file.name
        img_gray = cv2.imread(name, 0)
        ############################## Noise Section ################################
        if opration_type == "Add Noise":
            img_gray = img_gray/255
            if noise_type == "Salt&Papper":
                final_img = Filters.add_sp_noise(img_gray, (noise_ratio/100))
                plt.imsave('images\\final_img.png', final_img, cmap='gray')
            elif noise_type == "Gaussian":
                final_img = Filters.add_gaussian_noise(
                    img_gray, gaussuan_sigma)
                plt.imsave('images\\final_img.png', final_img, cmap='gray')
        ############################## Filter Section ###############################
        elif opration_type == "Filter":
            if filter_type == "Avrage Filter":
                final_img = Filters.avrage_filter2(img_gray, kernal_dim)
                plt.imsave('images\\final_img.png', final_img, cmap='gray')
            elif filter_type == "Gaussian Filter":
                final_img = Filters.gaussian_filter(
                    img_gray, kernal_dim, gaussuan_sigma)
                plt.imsave('images\\final_img.png', final_img, cmap='gray')
            elif filter_type == "Median Filter":
                final_img = Filters.median_filter(img_gray, kernal_dim)
                plt.imsave('images\\final_img.png', final_img, cmap='gray')
        ############################## Edge Section ##################################
        elif opration_type == "Edge Detection":
            if edge_type == "Roberts Operator":
                final_img = Filters.roberts_edge(img_gray)
                plt.imsave('images\\final_img.png', final_img, cmap='gray')
            elif edge_type == "Prewitt Operator":
                final_img = Filters.Prewitt_edge(img_gray)
                plt.imsave('images\\final_img.png', final_img, cmap='gray')
            elif edge_type == "Sobel Operator":
                final_img = Filters.Sobel_edge(img_gray, x_dir, y_dir)
                plt.imsave('images\\final_img.png', final_img, cmap='gray')
            elif edge_type == "Canny Operator":
                final_img = Filters.canny_edge(img_gray)
            plt.imsave('images\\final_img.png', final_img, cmap='gray')
        with before_col2:
            st.image(file, caption=None, width=500,
                     channels="RGB", output_format="auto")

        with after_col2:
            final_img = cv2.imread('images\\final_img.png', 0)
            st.image(final_img, caption=None, width=500,
                     channels="GRAY", output_format="auto")
            save = st.button("save")
            if save:
                name = 'images\\save_img' + \
                    format(np.random.randint(1, 100))+'.png'
                plt.imsave(name, final_img, cmap='gray')
with tab2:
    st.header("Histograms")
    st.sidebar.title("Select Image")
    file = st.sidebar.file_uploader(' ')
    before, TestEdit, after = tab2.columns(3, gap='large')
    before1, after1= tab2.columns(2, gap='large')
    # up, down = tab2.rows(2)
    # st.sidebar.button("Convert To Grey")
    if file is not None:
        data1 = cv2.imread('images\\'+file.name)
        print("ImageA ", data1.shape)
        data = cv2.imread('images\\'+file.name, 0)
        print("ImageA ", data.shape)
        hist = cv2.calcHist(data, [0], None, [256], [0, 256])
        # st.write('hist')
        # st.write(hist)
        # unq, cnt = np.unique(hist, axis=0, return_counts=True)
        # st.write('cnt')
        # st.write(cnt)
        # st.write('unq')
        # st.write(unq)
    if(st.sidebar.button("Histogram Equalization") and (file is not None)):
        with before1:
            st.image('images\\'+file.name, caption='Before')
            fig = plt.figure()
            plt.hist(data.ravel(), 256, [0, 256])
            # plt.hist(data.ravel())
            st.plotly_chart(fig)
            # st.title("Before")
            fig6 = plt.figure()
            # plt.plot(np.linspace(0,255, 256/len(equalized)),equalized)
            unique, counts = np.unique(data.ravel(), return_counts=True)
            # np.asarray((unique, counts)).T
            plt.plot(unique, counts)
            # sns.kdeplot(data.ravel())
            plt.title('Distribuation Curve Before Equalization')
            st.plotly_chart(fig6)
            a = Hs.drawCumulativeEq(data, 'original')
            st.plotly_chart(a)
        with after1:
            fig3 = plt.figure()
            # st.write(data)
            equalized = Hs.histEqualization(data, max(data.ravel()))
            plt.hist(equalized, 256, [0, 256])
            equalizedImage = np.reshape(equalized, data.shape)
            cv2.imwrite("images\\HistEqualized.png", equalizedImage)
            st.image("images\\HistEqualized.png", caption= 'After')
            st.plotly_chart(fig3)
            # st.title("After")
            # st.write("Equalized Distribution Curve")
            fig5 = plt.figure()
            # EqHist, EqBins = np.histogram(equalized, bins= 256, range=(0,1))
            # print("EQHIST ", EqHist)
            # plt.plot(EqBins[0:-1], EqHist)
            unique1, counts1 = np.unique(equalized, return_counts=True)
            # np.asarray((unique, counts)).T
            plt.plot(unique1, counts1)
            plt.title('Distribuation Curve After Equalization')
            st.plotly_chart(fig5)
            a = Hs.drawCumulativeEq(equalizedImage, 'equalized')
            st.plotly_chart(a)
    if(st.sidebar.button("Normalize")and (file is not None)):
        with before:
            norm = Hs.Normalize(data)
            cv2.imwrite("images\\Normalized.png", norm)
            # st.write("Normalized Image")
            st.image("images\\Normalized.png", caption='Normalized Image')
        with TestEdit:
            # st.write("Original")
            st.image('images\\'+file.name, caption='Original')
        with after:
            norm1 = cv2.normalize(data, None, alpha=0,beta=200, norm_type=cv2.NORM_MINMAX)
            cv2.imwrite("images\\NormalizedOpenCV.png", norm1)
            # st.write("Normalized Image with Open CV")
            st.image("images\\NormalizedOpenCV.png", caption='Normalized Image with Open CV')
    
    if(st.sidebar.button("Thresholding") and (file is not None)):
            with before:
                # st.write("Global Thresholding")
                array = Hs.Thresholding(data, 255, 0)
                cv2.imwrite("images\\Thresholded.png", array)
                st.image("images\\Thresholded.png", caption ='Global Thresholding')
                # st.write("Open CV Global Thresholding")
                ret, thresh = cv2.threshold(data, np.mean(data.ravel()), 255, cv2.THRESH_BINARY)
                cv2.imwrite("images\\Thresholded2.png", thresh)
                st.image("images\\Thresholded2.png", 'Open CV Global Thresholding')
            with TestEdit:
                # st.write("Local Thresholding")
                array1 = Hs.localThresholding(data, [2, 2])
                array1 = np.reshape(array1, data.shape)
                cv2.imwrite("images\\localThresholded.png", array1)
                st.image("images\\localThresholded.png",'Local Thresholding')
                st.image('images\\'+file.name, caption='Original Image')
            # ret, thresh = cv2.threshold(data, np.mean(data.ravel()), 255, cv2.THRESH_BINARY)
            with after:
                # st.write("Open CV Thresholding")
                th2 = cv2.adaptiveThreshold(data, 255, cv2.ADAPTIVE_THRESH_MEAN_C,
                                            cv2.THRESH_BINARY, 5, 5)
                cv2.imwrite("images\\Thresholded1.png", th2)
                st.image("images\\Thresholded1.png", "Open CV Thresholding")
    if(st.sidebar.button("Convert To Gray") and (file is not None)):
        if(len(data1.shape) == 3):
            with before1:
                st.image('images\\'+file.name, caption="The RGB Image",width = 475)
                unique2, counts2 = np.unique(data1[...,0], return_counts=True)
                print(data1[...,0].shape)
                fig0 = plt.figure()
                # plt.plot(unique2, counts2)
                # plt.hist(data1[...,0].ravel(), 256, [0, 256], color='r', alpha = 0.9)
                plt.hist(data1[...,0].ravel(), 256, [0, 256], color='r')
                st.plotly_chart(fig0, use_container_width=True)
            # with TestEdit:
                fig0 = plt.figure()
                plt.hist(data1[...,1].ravel(), 256, [0, 256], color = 'g') 
                st.plotly_chart(fig0, use_container_width=True)
                unique3, counts3 = np.unique(data1[...,1], return_counts=True)
                a = Hs.drawCumulative1(data1, 'red')
                st.plotly_chart(a, use_container_width=True)
                # a = Hs.drawCumulative(data1[...,1], 'green')
                # st.plotly_chart(a)

            # with after:
            with after1:
                greyImage = Hs.ToGrey(data1)
                print(data.shape)
                cv2.imwrite("images\\GreyScale.png", greyImage)
                st.image("images\\GreyScale.png",caption="The Gray Scale Image", width = 475)
                fig0 = plt.figure()
                plt.hist(data1[...,2].ravel(), 256, [0, 256], color='b')
                st.plotly_chart(fig0, use_container_width=True)
                unique4, counts4 = np.unique(data1[...,2], return_counts=True)
                fig0 = plt.figure()
                plt.plot(unique2, counts2, color='red')
                plt.plot(unique3, counts3, color='green')
                plt.plot(unique4, counts4, color='blue')
                plt.legend(['Distribution curve of Red', 'Distribution curve of Green', 'Distribution curve of Blue'])
                st.plotly_chart(fig0, use_container_width=True)
                # a = Hs.drawCumulative(data1[...,2], 'blue')
                # st.plotly_chart(a)
                # st.plotly_chart(b)


        else:
            st.error("This image is already a gray scale image")

with tab3:
    st.header("Frequency")
    col1, col2 ,col3= st.columns([2, 2, 4])
    with col1:
        uploaded_image1 = st.file_uploader("Low_pass Image",  key=1)
    with col2:
        uploaded_image2 = st.file_uploader("High_pass Image", key=2)
        

    if uploaded_image1 is not None:

        # save_path = Path(images_folder, uploaded_image1.name)
        # freq_pic1 = 'images\\'+uploaded_image1.name
        # with open(save_path, mode='wb') as w:
        #     w.write(uploaded_image1.getvalue())
        file1 = 'images\\'+uploaded_image1.name

    if uploaded_image2 is not None:
        # save_path = Path(images_folder, uploaded_image2.name)
        # freq_pic2 = 'images\\'+uploaded_image2.name
        # with open(save_path, mode='wb') as w:
        #     w.write(uploaded_image2.getvalue())
        file2 = 'images\\'+uploaded_image2.name

    if uploaded_image1 is not None:
        if uploaded_image2 is not None:
            image1 = cv2.imread(file1, 0)
            image2 = cv2.imread(file2, 0)
 
            with col3:

                sigma = st.slider("standard deviation", min_value=0.1, max_value=20.0,value=10.0, step=0.1,  label_visibility="visible")
                freq.fft_hyprid_image(image1,image2,sigma)

            original, filter, result = st.columns([2, 2, 4])
            with original :
                st.image('debug/original_low.jpg')
                st.image('debug/original_high.jpg')

            with filter :
                st.image('debug/lgauss.jpg')
                st.image('debug/hgauss.jpg')
              
            with result:
                
                empty3, hybrid_col, empty4 = st.columns([1, 7, 1])
                with hybrid_col:
                    empty1, text, empty2 = st.columns([1.5, 3, 1])
                    with text :
                        st.subheader('Hybrid Image')
                    st.image('debug/gauss.jpg')

with tab4:
    
    st.header("Active contour")
    file = st.file_uploader("Active Image",key=10)
    alpha_slider, beta_slider, gamma_slider = st.columns(3)
    before_col, after_col   = tab4.columns([1, 1])



    if file is not None:

        with alpha_slider:
                    alpha = st.slider("Alpha",  min_value=0.0, max_value=2.0,
                                           value=1.0, step=0.1,  label_visibility="visible",key='a')
        with beta_slider:
                    beta = st.slider("Beta", min_value=0.0, max_value=2.0,
                                           value=1.0, step=0.1,  label_visibility="visible",key='b')
        with gamma_slider:
                    gamma = st.slider("Gamma", min_value=0.0, max_value=2.0,
                                           value=1.0, step=0.1,  label_visibility="visible",key='g')
        name = 'activecontour_images/'+file.name
        img_gray = cv2.imread(name, 0)

        contour_points = [(88,156),(70,106),(144,151),(160,156),(170,110),(190,156),(205,155),(241,155),(189,224),(135,235)]

        img = np.array(img_gray)          #reading image
        lap = np.array(img_laplacian(img))		#apply canny 
        lap = add_border(lap)					#padding image

        contour = Contour(contour_points) #passing the contour points to contour class

        # Create series of images fitting contour
        allimgs = []
        iteration_num =100
        for i in range(iteration_num):
                
            lapcpy = np.copy(lap) 
            contour.calc_energies(lapcpy,alpha,beta,gamma)
            contour.update_points()
            contour.draw_contour(lapcpy)  # drawing contour points 
            allimgs.append(lapcpy)		  #end of loop result

        with before_col:
            st.image(name)
            arr=np.array(contour_points)
            area=cv2.contourArea(arr)
            primeter=cv2.arcLength(arr,closed=True)
            st.write(f"intial primeter:{primeter}")
            st.write(f"intial area:{area}")

        with after_col:
            cv2.imwrite('activecontour_images/result_active.png' ,allimgs[-1])
            st.image('activecontour_images/result_active.png' )
            final=contour.get_contour_points()
            arr=np.array(final)
            area=cv2.contourArea(arr)
            primeter=cv2.arcLength(arr,closed=True)
            st.write(f"final primeter:{primeter}")
            st.write(f"final area:{area}")



        st.write ("Chain Code represntation")
        code =contour.get_chain_code()
        st.write (code)

 
    
