using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Collections;
using System.Diagnostics;

namespace HuffmanCoding
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        public struct Node//定义节点结构
        {
            public int weight; //定义权重
            public int parent, lchild, rchild;//定义父亲，孩子 
        };
        public struct Min//定义最小两位数结构
        {
            public int s1;
            public int s2;
        };

        Node[] HTN = new Node[500];
        Min Getmin(int n) //寻找最小的两位权值
        {
            int min1, min2, i;
            Min code; //定义结构体Min
            code.s1 = 1;
            code.s2 = 1;
            min1 = 256;
            min2 = 256;
            for (i = 0; i < n; i++)//寻找最小值 
            {
                if (HTN[i].weight <= min1 && HTN[i].parent == 0)
                {
                    min1 = HTN[i].weight;
                    code.s1 = i;
                }
            }
            for (i = 0; i <= n; i++)//寻找次最小值 
            {
                if (HTN[i].weight <= min2 && i != code.s1 && HTN[i].parent == 0)
                {
                    min2 = HTN[i].weight;
                    code.s2 = i;
                }
            }
            return code;
        }
        private void Form1_Load(object sender, EventArgs e)
        {


        }
        private void button1_Click(object sender, EventArgs e)//编码响应
        {
            listBox1.Items.Clear();
            int length;//统计输入的字符个数
            int Num = 0;//统计输入字符的种类
            int i, j, k, m, start, c, f;
            Min min;
            int[] weight = new int[256];//用于统计每次字符出现的次数
            int[] ind = new int[256];//用于统计i对应的字符
            int[] wei = new int[256];//用于统计每个字符权重
            string Input = richTextBox1.Text.Trim();//输入的编码字符存到Input数组中
            string code = "";
            if (richTextBox1.Text == "") MessageBox.Show("请输入编码字符！");//显示提示符输入字符
            else
            {
                //编码开始
                System.Diagnostics.Stopwatch stopwatch = new System.Diagnostics.Stopwatch();
                stopwatch.Start();//开始计算时间

                length = richTextBox1.Text.Length;
                for (i = 0; i < length; i++)
                    weight[Input[i]]++; //统计每个字符出现的次数即权重
                for (i = 0; i < 256; i++)
                {
                    if (weight[i] != 0) //若个数不为0，将这个字符存入ind数组中，
                    {
                        ind[Num] = i; //Ind存放字符代码 
                        wei[Num++] = weight[i];//相应的权重存放在weight数组中，记录个数输入的字符有num种
                    }
                }

                m = 2 * Num - 1;
                for (i = 0; i < Num; i++)//初始化
                {
                    HTN[i].weight = wei[i];
                    HTN[i].parent = 0;
                    HTN[i].lchild = 0;
                    HTN[i].rchild = 0;
                }
                for (; i < m; i++)
                {
                    HTN[i].weight = 0;
                    HTN[i].parent = 0;
                    HTN[i].lchild = 0;
                    HTN[i].rchild = 0;
                }
                for (i = Num; i < m; i++)//建树
                {
                    min = Getmin(i - 1); //找出最小的两位数
                    HTN[min.s1].parent = i;
                    HTN[min.s2].parent = i;
                    HTN[i].lchild = min.s1;
                    HTN[i].rchild = min.s2;
                    HTN[i].weight = HTN[min.s1].weight + HTN[min.s2].weight;
                }

                string[] HC = new string[Num];
                char[] cd = new char[Num + 5];
                for (i = 0; i < Num; i++)//对每个字符进行编码
                {
                    start = Num + 5;
                    for (c = i, f = HTN[i].parent; f != 0; c = f, f = HTN[f].parent)
                    {
                        if (HTN[f].lchild == c) cd[--start] = '0';
                        else cd[--start] = '1';
                    }
                    for (j = start; j < Num + 5; j++)
                        HC[i] += cd[j];
                }
                for (i = 0; i < length; i++)//将输入字符编程码
                {
                    k = 0;
                    int flag = 0;
                    for (j = 0; j < Num && flag == 0; j++)
                    {
                        if (ind[j] == Input[i])//寻找该字符对应的编码
                        {
                            k = j;
                            flag = 1;
                        }
                    }
                    code += HC[k].ToString();
                }
                //字符编码结束
                stopwatch.Stop(); //  停止监视
                TimeSpan timeSpan = stopwatch.Elapsed; //  获取总时间            
                double milliseconds = timeSpan.TotalMilliseconds;  //  毫秒数
                
                int bits=0;
                
                //显示各个结果
                richTextBox2.Text = code;//最终编码输出至richTextBox2中
                for (i = 0; i < Num; i++)//将每个字符编码输出至listbox中
                {
                    bits +=Convert.ToString(HC[i]).ToString().Length;//得到总编码位数
                    this.listBox1.Items.Add(Convert.ToString(Convert.ToChar(ind[i])) + "：  " + HC[i]);  
                }
               
            this.listBox1.Items.Add("\n运行时间：" + Convert.ToString(milliseconds)+"ms");
            this.listBox1.Items.Add("\n效率："+(double)(8*Num-bits)/(8*Num)  );//计算压缩比
            }         

        }



    }
}
