
using OpenCvSharp;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Drawing.Imaging;
using System.Linq;
using System.Text;
using System.Windows.Forms;


//显示图片
namespace showPicture
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }
        private string pathname = string.Empty;     		//定义路径名变量
        private void button1_Click(object sender, EventArgs e)  	//打开方法
        {
            OpenFileDialog file = new OpenFileDialog();
            file.InitialDirectory = ".";
            file.Filter = "所有文件(*.*)|*.*";
            file.ShowDialog();
            if (file.FileName != string.Empty)
            {
                try
                {
                    pathname = file.FileName;   //获得文件的绝对路径
                    pictureBox1.Load(pathname);
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message);
                }
            }

        }


        private void button2_Click_1(object sender, EventArgs e)
        {
            SaveFileDialog save = new SaveFileDialog();
            save.ShowDialog();
            if (save.FileName != string.Empty)
            {
                pictureBox1.Image.Save(save.FileName);
            }
        }

        private void button3_Click(object sender, EventArgs e)
        {
            Bitmap ToGray(Bitmap bmp)
            {
                for (int i = 0; i < bmp.Width; i++)
                {
                    for (int j = 0; j < bmp.Height; j++)
                    {
                        //获取该点的像素的RGB的颜色
                        Color color = bmp.GetPixel(i, j);
                        //利用公式计算灰度值
                        int gray = (int)(color.R * 0.3 + color.G * 0.59 + color.B * 0.11);
                        Color newColor = Color.FromArgb(gray, gray, gray);
                        bmp.SetPixel(i, j, newColor);
                    }
                }
                return bmp;
            }
            //读取bitmap文件，在第一个控件已经完成了pathname的读取所以直接传pathname参数
            Bitmap btmp = new Bitmap(pathname);
            //显示图片
            pictureBox2.Image = ToGray(btmp);
        }


        //二值化
        private void button4_Click(object sender, EventArgs e)
        {
            //迭代法计算阈值
            Bitmap Thresholding(Bitmap img1)
            {
                int[] histogram = new int[256];
                int minGrayValue = 255, maxGrayValue = 0;
                //求取直方图
                for (int i = 0; i < img1.Width; i++)
                {
                    for (int j = 0; j < img1.Height; j++)
                    {
                        Color pixelColor = img1.GetPixel(i, j);
                        histogram[pixelColor.R]++;
                        if (pixelColor.R > maxGrayValue) maxGrayValue = pixelColor.R;
                        if (pixelColor.R < minGrayValue) minGrayValue = pixelColor.R;
                    }
                }
                //迭代计算阀值
                int threshold = -1;
                int newThreshold = (minGrayValue + maxGrayValue) / 2;
                for (int iterationTimes = 0; threshold != newThreshold && iterationTimes < 100; iterationTimes++)
                {
                    threshold = newThreshold;
                    int lP1 = 0;
                    int lP2 = 0;
                    int lS1 = 0;
                    int lS2 = 0;
                    //求两个区域的灰度的平均值
                    for (int i = minGrayValue; i < threshold; i++)
                    {
                        lP1 += histogram[i] * i;
                        lS1 += histogram[i];
                    }
                    int mean1GrayValue = (lP1 / lS1);
                    for (int i = threshold + 1; i < maxGrayValue; i++)
                    {
                        lP2 += histogram[i] * i;
                        lS2 += histogram[i];
                    }
                    int mean2GrayValue = (lP2 / lS2);
                    newThreshold = (mean1GrayValue + mean2GrayValue) / 2;
                }
                //计算二值化
                for (int i = 0; i < img1.Width; i++)
                {
                    for (int j = 0; j < img1.Height; j++)
                    {
                        Color pixelColor = img1.GetPixel(i, j);
                        if (pixelColor.R > threshold)
                            img1.SetPixel(i, j, Color.FromArgb(255, 255, 255));
                        else img1.SetPixel(i, j, Color.FromArgb(0, 0, 0));
                    }
                }

                return img1;
            }
            Bitmap btmp = new Bitmap(pathname);
            pictureBox2.Image = Thresholding(btmp);
        }


        private void button5_Click(object sender, EventArgs e)
        {

            Bitmap pictureProcess(Bitmap sourceImage, int targetWidth, int targetHeight)
            {
                try
                {
                    ImageFormat format = sourceImage.RawFormat;
                    Bitmap targetPicture = new Bitmap(targetWidth, targetHeight);
                    Graphics g = Graphics.FromImage(targetPicture);
                    g.DrawImage(sourceImage, 0, 0, targetWidth, targetHeight);
                    sourceImage.Dispose();

                    return targetPicture;
                }
                catch (Exception ex)
                {
                    throw ex;
                }

            }
            int h = Convert.ToInt32(textBox1.Text);
            int w = Convert.ToInt32(textBox2.Text);
            Bitmap img = new Bitmap(pathname);
            pictureBox2.Image = pictureProcess(img, w, h);

        }



        private void button6_Click(object sender, EventArgs e)
        {
            Rectangle GetRotateRectangle(int width, int height, float angle)
            {
                double radian = angle * Math.PI / 180; ;
                double cos = Math.Cos(radian);
                double sin = Math.Sin(radian);
                //只需要考虑到第四象限和第三象限的情况取大值(中间用绝对值就可以包括第一和第二象限)
                int newWidth = (int)(Math.Max(Math.Abs(width * cos - height * sin), Math.Abs(width * cos + height * sin)));
                int newHeight = (int)(Math.Max(Math.Abs(width * sin - height * cos), Math.Abs(width * sin + height * cos)));
                return new Rectangle(0, 0, newWidth, newHeight);
            }


            Bitmap GetRotateImage(Bitmap srcImage, int angle)
            {
                angle = angle % 360;
                //原图的宽和高
                int srcWidth = srcImage.Width;
                int srcHeight = srcImage.Height;
                //图像旋转之后所占区域宽和高
                Rectangle rotateRec = GetRotateRectangle(srcWidth, srcHeight, angle);
                int rotateWidth = rotateRec.Width;
                int rotateHeight = rotateRec.Height;
                //目标位图
                Bitmap destImage = null;
                Graphics graphics = null;
                try
                {
                    //定义画布，宽高为图像旋转后的宽高
                    destImage = new Bitmap(rotateWidth, rotateHeight);
                    //graphics根据destImage创建，因此其原点此时在destImage左上角
                    graphics = Graphics.FromImage(destImage);
                    //要让graphics围绕某矩形中心点旋转N度，分三步
                    //第一步，将graphics坐标原点移到矩形中心点,假设其中点坐标（x,y）
                    //第二步，graphics旋转相应的角度(沿当前原点)
                    //第三步，移回（-x,-y）
                    //获取画布中心点
                    System.Drawing.Point centerPoint = new System.Drawing.Point(rotateWidth / 2, rotateHeight / 2);
                    //将graphics坐标原点移到中心点
                    graphics.TranslateTransform(centerPoint.X, centerPoint.Y);
                    //graphics旋转相应的角度(绕当前原点)
                    graphics.RotateTransform(angle);
                    //恢复graphics在水平和垂直方向的平移(沿当前原点)
                    graphics.TranslateTransform(-centerPoint.X, -centerPoint.Y);
                    //此时已经完成了graphics的旋转

                    //计算:如果要将源图像画到画布上且中心与画布中心重合，需要的偏移量
                    System.Drawing.Point Offset = new System.Drawing.Point((rotateWidth - srcWidth) / 2, (rotateHeight - srcHeight) / 2);
                    //将源图片画到rect里（rotateRec的中心）
                    graphics.DrawImage(srcImage, new Rectangle(Offset.X, Offset.Y, srcWidth, srcHeight));
                    //重至绘图的所有变换
                    graphics.ResetTransform();
                    graphics.Save();
                }
                catch (Exception ex)
                {
                    throw ex;
                }
                finally
                {
                    if (graphics != null)
                        graphics.Dispose();
                }
                return destImage;
            }
            Bitmap img = new Bitmap(pathname);
            int an = Convert.ToInt32(textBox3.Text.Trim());
            pictureBox2.Image = GetRotateImage(img, an);

        }



        private void button7_Click(object sender, EventArgs e)
          {
            Bitmap offsetp(Bitmap a, int s, int v)
            {

                System.Drawing.Imaging.BitmapData srcData = a.LockBits(new Rectangle(0, 0, a.Width, a.Height), System.Drawing.Imaging.ImageLockMode.ReadWrite, a.PixelFormat);
                IntPtr ptr = srcData.Scan0;
                int bytes = srcData.Stride * a.Height;
                byte[] grayVlaues = new byte[bytes];
                System.Runtime.InteropServices.Marshal.Copy(ptr, grayVlaues, 0, bytes);
                byte[] tempArray = new byte[bytes];
                for (int i = 0; i < bytes; i++)
                {
                    tempArray[i] = 255;
                }
                for (int i = 0; i < a.Width * 3; i += 3)
                {
                    if ((i + s * 3) < a.Width * 3 && (i + s * 3) > 0)
                    {
                        for (int j = 0; j < a.Height; j++)
                        {
                            if ((j + v) < a.Height && (j + v) > 0)
                            {
                                tempArray[(i + s * 3) + (j + v) * srcData.Stride] = grayVlaues[i + j * srcData.Stride];
                                tempArray[i + s * 3 + 1 + (j + v) * srcData.Stride] = grayVlaues[i + 1 + j * srcData.Stride];
                                tempArray[i + s * 3 + 2 + (j + v) * srcData.Stride] = grayVlaues[i + 2 + j * srcData.Stride];
                            }
                        }
                    }
                }
                grayVlaues = (byte[])tempArray.Clone();
                System.Runtime.InteropServices.Marshal.Copy(grayVlaues, 0, ptr, bytes);
                a.UnlockBits(srcData);
                return a;
            }
                Bitmap img = new Bitmap(pathname);

                int x = Convert.ToInt32(textBox5.Text.Trim());
                int y = Convert.ToInt32(textBox4.Text.Trim());
                pictureBox2.Image = offsetp(img, x, y);         
          }
    }
        
    
}