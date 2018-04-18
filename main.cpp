// Model.cpp : Defines the entry point for the console application.
//

#include "part.h"
#include "order.h"
#include <regex>
#include<stdlib.h>
#include<cmath>
#include<at_pos.hxx>
#include<at_vec.hxx>
#include <heal_api.hxx>
#include <insanity_list.hxx> 
//#include <tolerize_ent.hxx>     
//#include <cover_options.hxx> 
//#include<coverapi.hxx>
//#include "ACISModel.h"
//#include "all_h.h"
//#define FLAG  ("FQHWIFBSCIEWFBSHFfhkFNEWIQUFBDA1343421312SFWFIFINFKASFHSDAFEWFFCE")
using namespace std;

int main()
{
	//这是常规项，用以测试model.exe
	/*Part part;
	part.Draw("D:\\Documents\\FMT\\FMT-5.5\\release\\test_path\\uselib\\result.nps", 
		"C:\\Users\\Jonathan_Lewis\\Desktop\\new\\","D:\\Documents\\FMT\\FMT-5.5\\release\\test_path\\uselib");*/

	//下面是测试各种函数的，可以注释掉
	unlock_spatial_products_2541();
	InitACIS();
	
	/*void test_skin();
	test_skin();

	void test_cone();
	test_cone();

	void test_sphere();
	test_sphere();*/
	
	//void test_cover();
	//test_cover();
	//void test_sphere1();      
	//test_sphere1();

	/*void read_data();
	read_data();*/

	/*void test_attrib();
	test_attrib();*/

	/*void test_assem();
	test_assem();*/

	void testEllipsoid();
	testEllipsoid();

	 AcisModelExit();
	return 0;
}

//
void testEllipsoid(){
	BODY* sphere1,*sphere,*bool_body;
	api_set_int_option( "new_periodic_splitting", 3 );
	//api_solid_sphere(SPAposition(0,0,0),5,sphere);//这里不是原点的圆心的话，后面拉伸，椭圆心会移动
	api_solid_sphere(SPAposition(0,0,0),10,sphere1);
	//api_transform_entity(sphere,scale_transf(1,1,2));
	SPAtransf x = scale_transf(0.5,0.5,1);
	TRANSFORM * y = new TRANSFORM(x);
	api_change_body_trans( sphere1, y) ;
	api_transform_entity(sphere1,scale_transf(0.5,0.5,1));
	//api_transform_entity(sphere1,rotate_transf(180,SPAvector(0,1,0)));
	api_change_body_trans( sphere1, NULL ) ;

	//api_boolean(sphere, sphere1, UNION, NDBOOL_KEEP_NEITHER, bool_body);

	//api_initialize_healing();
	//api_hh_init_body_for_healing(sphere1);
	//api_hh_auto_heal(sphere1);
	//api_hh_end_body_for_healing(sphere1);
	//api_terminate_healing();
	ENTITY_LIST pModelEntityList;

	insanity_list *list;
	api_check_entity(sphere1,list);
	cout<<list->count();
	FILE * fp;
	fp=fopen("C:\\Users\\Jonathan_Lewis\\Desktop\\error.txt","r");
	list->print_messages(fp);


	pModelEntityList.init();
	//pModelEntityList.add(sphere);
	pModelEntityList.add(sphere1);
	//pModelEntityList.add(bool_body);
	SaveSatFile("C:\\Users\\Jonathan_Lewis\\Desktop\\testEllipsoid1.sat",pModelEntityList);
} 

//在部件ent里面寻找名为facename的面，返回其法向量和中心点坐标
//void find_face(string facename,ENTITY* ent, SPAvector & facevec,SPAposition & facepos){
//	ATTRIB_GEN_NAME *na;
//
//	string vec_name = facename+":vec";
//	api_find_named_attribute(ent,vec_name.c_str(),na);
//	ATTRIB_GEN_VECTOR* vec=(ATTRIB_GEN_VECTOR*)na;
//	if(na == NULL){
//		cout<<"未找到面"<<facename<<"的法向量"<<endl;
//		return;
//	}
//	cout<<na->name()<<endl;
//	facevec=vec->value();
//	cout<<"("<<vec->value().x()<<","<<vec->value().y()<<","<<vec->value().z()<<")"<<endl;
//
//	string pos_name = facename+":pos";
//	api_find_named_attribute(ent,pos_name.c_str(),na);
//	if(na == NULL){
//		cout<<"未找到面"<<facename<<"的中心点坐标"<<endl;
//		return;
//	}
//	cout<<na->name()<<endl;
//	ATTRIB_GEN_POSITION* pos=(ATTRIB_GEN_POSITION*)na;
//	facepos=pos->value();
//	cout<<"("<<pos->value().x()<<","<<pos->value().y()<<","<<pos->value().z()<<")"<<endl;
//}
//
//SPAvector cross_product(SPAvector a,SPAvector b){//计算三维矢量积
//	double x,y,z;
//	x=a.y()*b.z()-b.y()*a.z();
//	y=-(a.x()*b.z()-a.z()*b.x());
//	z=a.x()*b.y()-a.y()*b.x();
//	return SPAvector(x,y,z);
//}
//
//double vector_angle(SPAvector a,SPAvector b){//计算矢量夹角
//	double a_;
//	a_=sqrt(a.x()*a.x()+a.y()*a.y()+a.z()*a.z());
//	double b_=sqrt(b.x()*b.x()+b.y()*b.y()+b.z()*b.z());
//	double theta = acos((a.x()*b.x()+a.y()*b.y()+a.z()*b.z())/(a_*b_));
//	return theta;
//}

/*计算点orig绕经过原点的矢量axis，逆时针旋转弧度theta后的点坐标，
参考文章https://blog.csdn.net/gamesdev/article/details/9929211
和https://blog.csdn.net/qiuchangyong/article/details/5859628*/
//SPAposition rotated_pos(SPAposition orig,SPAvector axis,double angle){
//	double c=cos(angle);
//	double s=sin(angle);
//	double x0=orig.x();
//	double y0=orig.y();
//	double z0=orig.z();
//	//cout<<"("<<axis.x()<<","<<axis.y()<<","<<axis.z()<<")"<<endl;
//	SPAvector unit_axis=axis/sqrt(axis.x()*axis.x()+axis.y()*axis.y()+axis.z()*axis.z());//单位化axis矢量
//	double i=unit_axis.x();
//	double j=unit_axis.y();
//	double k=unit_axis.z();
//	//cout<<"("<<i<<","<<j<<","<<k<<")"<<endl;
//	double x1= ((1-c)*i*i+c)*x0+((1-c)*i*j-s*k)*y0+((1-c)*i*k+s*j)*z0;
//	double y1=((1-c)*i*j+s*k)*x0+((1-c)*j*j+c)*y0+((1-c)*j*k-s*i)*z0;
//	double z1=((1-c)*i*k-s*j)*x0+((1-c)*j*k+s*i)*y0+((1-c)*k*k+c)*z0;
//	return SPAposition(x1,y1,z1);
//}
//void test_attrib(){
//	BODY* cyl;
//	SPAposition A(0,0,0),B(0,4,5);
//	api_solid_cylinder_cone(A,B,2.5,3,2.5,NULL,cyl);
//	ENTITY_LIST pModelEntityList;
//	 pModelEntityList.init();
//	 pModelEntityList.add(cyl);
//	api_add_generic_named_attribute(cyl,"S1:pos",A);
//	api_add_generic_named_attribute(cyl,"S1:vec",B-A);
//	SaveSatFile("C:\\Users\\Jonathan_Lewis\\Desktop\\cyl.sat",pModelEntityList);
//
//	BODY* cone;
//	SPAposition C(0,0,0),D(0,6,3);
//	api_solid_cylinder_cone(C,D,5,4,0,NULL,cone);
//	pModelEntityList.clear();
//	 pModelEntityList.add(cone);
//	api_add_generic_named_attribute(cone,"S1:pos",C);
//	api_add_generic_named_attribute(cone,"S1:vec",D-C);
//	SaveSatFile("C:\\Users\\Jonathan_Lewis\\Desktop\\cone.sat",pModelEntityList);
//
//	ENTITY_LIST list;
//	//ENTITY* ent;
//	//ATTRIB* att;
//	ReadSatFile("C:\\Users\\Jonathan_Lewis\\Desktop\\cyl.sat",list);
//	ReadSatFile("C:\\Users\\Jonathan_Lewis\\Desktop\\cone.sat",list); 
//	SPAvector cylV,coneV;
//	SPAposition cylP,coneP;
//	find_face("S1",list[0],cylV,cylP);
//	find_face("S1",list[1],coneV,coneP);
//	//让圆锥的S1面跟圆柱的S1面共面，法向量相反
//	//coordinate_transf(cylP,)
//	SaveSatFile("C:\\Users\\Jonathan_Lewis\\Desktop\\before_trans.sat",list);
//
//	//bool reverse=true;//共面的时候，法向量方向相同还是相反，默认相反
//	double angle=vector_angle(cylV,-coneV);
//	SPAvector axis=cross_product(cylV,coneV);
//	api_transform_entity(list[1],rotate_transf(angle,axis));//法向量相反(coneV=-coneV则法向量同向）
//	api_transform_entity(list[1],translate_transf(cylP-rotated_pos(coneP,axis,angle)));//将两个面的中心点移至同一个地方
//	
//	api_add_generic_named_attribute(list[1],"S1:pos",cylP);
//	api_add_generic_named_attribute(list[1],"S1:vec",-cylV);
//	//cout<<vector_angle(cylV,coneV)<<endl<<cross_product(cylV,coneV).x()<<cross_product(cylV,coneV).y()<<cross_product(cylV,coneV).z();
//	SaveSatFile("C:\\Users\\Jonathan_Lewis\\Desktop\\trans.sat",list);  
//}

//}
//void test_assem(){
//	ENTITY_LIST list;
//	ReadSatFile("C:\\Users\\Jonathan_Lewis\\Desktop\\new\\assem.sat",list); //这个list是这样的：sat里面有几个单独的body，list就有多少个ENTITY
//	ATTRIB_GEN_NAME *na;
//	api_find_named_attribute(list[1],"名称",na);
//	ATTRIB_GEN_STRING *v=(ATTRIB_GEN_STRING *)na;
//	string val=v->value();
//	api_find_named_attribute(list[0],"材料",na);
//	v=(ATTRIB_GEN_STRING *)na;
//	val=v->value();
//	vector<vector<int> > l;
//	l.size();
//	l[0].size();
//	
//	}
//void split_number(string line, vector<double> &split) {
//	const regex num_pattern("\\S+");
//	smatch s;
//	while(std::regex_search(line,s,num_pattern)){
//	split.push_back(std::atof(s[0].str().c_str()));
//	line=s.suffix().str();
//	//cout<<line<<"  ;\n";	
//	}
//}
//void read_data()
//{
//	ifstream MeshVertex,MeshDisp,MeshRegion;
//	MeshVertex.open("D:\\Documents\\graduate2\\mesh\\Cylinder\\MeshVertex.txt",ios::in);
//	MeshDisp.open("D:\\Documents\\graduate2\\mesh\\Cylinder\\MeshDisplacement.txt",ios::in);
//	MeshRegion.open("D:\\Documents\\graduate2\\mesh\\Cylinder\\MeshRegion.txt",ios::in);
//	MeshVertex.seekg(0,ios::beg);
//	MeshDisp.seekg(0,ios::beg);
//	MeshRegion.seekg(0,ios::beg);
//	string line,lineD;
//	if(!MeshVertex || !MeshRegion){
//		cerr<<"文件打开失败！"<<endl;
//		abort();
//	}
//	vector<double> x,y,z;
//	while(!MeshVertex.eof()){
//		getline(MeshVertex,line);//这个地方后续需要验证line符合格式
//		vector<double> split,splitD;
//		split_number(line,split);
//		if(MeshDisp){
//			getline(MeshDisp,lineD);
//			split_number(lineD,splitD);
//			x.push_back(split[0]+splitD[0]);//这个地方后续续验证索引没有溢出
//			y.push_back(split[1]+splitD[1]);
//			z.push_back(split[2]+splitD[2]);
//		}
//		else {
//			x.push_back(split[0]);
//			y.push_back(split[1]);
//			z.push_back(split[2]);
//		}
//		//cout<<x.back()<<" "<<y.back()<<" "<<z.back()<<endl;
//	}
//	if(MeshDisp) 
//		MeshDisp.close();
//	MeshVertex.close();
//
//	ENTITY_LIST pModelEntityList;
//	pModelEntityList.init();
//	while(!MeshRegion.eof()){
//		getline(MeshRegion,line);
//		vector<double> splitR;//splitR 存放了一个网格的四个点的索引
//		split_number(line,splitR);
//
//		SPAposition A(x[splitR[0]],y[splitR[0]],z[splitR[0]]);
//		SPAposition B(x[splitR[1]],y[splitR[1]],z[splitR[1]]);
//		SPAposition C(x[splitR[2]],y[splitR[2]],z[splitR[2]]);
//		SPAposition D(x[splitR[3]],y[splitR[3]],z[splitR[3]]);
//
//		/*SPAposition B1(x[splitR[1]],y[splitR[1]]+0.005,z[splitR[1]]);
//		SPAposition C1(x[splitR[2]],y[splitR[2]]+0.005,z[splitR[2]]);
//		SPAposition D1(x[splitR[3]],y[splitR[3]]+0.005,z[splitR[3]]);*/
//
//		/*cout<<A.x()<<A.y()<<A.z()<<endl;
//		cout<<B.x()<<B.y()<<B.z()<<endl;
//		cout<<C.x()<<C.y()<<C.z()<<endl;
//		cout<<D.x()<<D.y()<<D.z()<<endl;*/
//		BODY* point_body,*line_body/*,*line_body1,*line_body2,*line_body3*/;
//		EDGE** triangle = new EDGE*[3];
//		//EDGE* curve;
//		SPAposition pts[1]={A};
//		api_build_wire(NULL,FALSE,1,pts,NULL,point_body);
//
//		api_curve_line(B,C,triangle[0]);
//		api_curve_line(C,D,triangle[1]);
//		api_curve_line(D,B,triangle[2]);
//		/*api_curve_line(A,B,triangle[3]);
//		api_curve_line(A,C,triangle[4]);
//		api_curve_line(A,D,triangle[5]);*/
//
//
//	/*	api_curve_arc_3pt(B1,C1,D1,true,curve);
//		api_make_ewire(1,&curve,point_body);*/
//
//		api_make_ewire(3,triangle,line_body);
//		/*api_make_ewire(1,triangle+3,line_body1);
//		api_make_ewire(1,triangle+4,line_body2);
//		api_make_ewire(1,triangle+5,line_body3);*/
//		//api_make_ewire(3,triangle,line_body);
//		BODY* bodies[2] = {line_body,point_body};
//		BODY* result;
//		api_skin_wires(2,bodies,result,0,1,1,1,1,1,0);
//		pModelEntityList.add(result);
//		//pModelEntityList.add(line_body);
//		/*pModelEntityList.add(line_body1);
//		pModelEntityList.add(line_body2);
//		pModelEntityList.add(line_body3);*/
//		//pModelEntityList.add(point_body);
//		break;
//	}
//		SaveSatFile("C:\\Users\\Jonathan_Lewis\\Desktop\\Mesh.sat",pModelEntityList);
//}
//void test_skin(){
//	EDGE* circle_edge;
//	EDGE* circle_edge1;
//	BODY* circle_body;
//	BODY* circle_body1;
//	SPAposition ptcenter(0,0,0),ptcenter1(0,5,10);
//
//	api_curve_arc(ptcenter,3,0,2*3.141592654,circle_edge);
//	api_make_ewire(1,&circle_edge,circle_body);
//
//	api_curve_arc(ptcenter1,1,0,2*3.141592654,circle_edge1);
//	api_make_ewire(1,&circle_edge1,circle_body1);
//
//	BODY* defaul[2]={ circle_body, circle_body1};
//	BODY* jonabody;
//	api_skin_wires(2,defaul,jonabody);
//	ENTITY_LIST pModelEntityList;
//	 pModelEntityList.init();
//	 pModelEntityList.add(jonabody);
//	SaveSatFile("C:\\Users\\Jonathan_Lewis\\Desktop\\testSkin.sat",pModelEntityList);
//}

//void test_cone(){
//	EDGE* circle_edge,*circle_edge1,*circle_edge2,*circle_edge3,*circle_edge4,*circle_edge5;
//	BODY* circle_body,* circle_body1,* circle_body2,* circle_body3,* circle_body4,*circle_body5,*point_body;
//	SPAposition A(0,0,12),B(0,0,10),C(0,0,8),D(0,0,6),E(0,0,4),F(0,0,2),G(0,0,0);
//
//	SPAposition pts[1] = {A};
//	api_build_wire (NULL, FALSE, 1, pts,NULL, point_body);
//
//	api_curve_arc(B,5.0/6.0,0,2*3.141592654,circle_edge);
//	api_make_ewire(1,&circle_edge,circle_body);
//
//	api_curve_arc(C,5.0/3.0,0,2*3.141592654,circle_edge1);
//	api_make_ewire(1,&circle_edge1,circle_body1);         
//
//	api_curve_arc(D,5.0/2.0,0,2*3.141592654,circle_edge2);
//	api_make_ewire(1,&circle_edge2,circle_body2);
//
//	api_curve_arc(E,10.0/3.0,0,2*3.141592654,circle_edge3);
//	api_make_ewire(1,&circle_edge3,circle_body3);
//
//	api_curve_arc(F,25.0/6.0,0,2*3.141592654,circle_edge4);
//	api_make_ewire(1,&circle_edge4,circle_body4);
//
//	api_curve_arc(G,5.0,0,2*3.141592654,circle_edge5);
//	api_make_ewire(1,&circle_edge5,circle_body5);
//
//	BODY* bodies[7] = {point_body,circle_body,circle_body1,circle_body2,circle_body3,circle_body4,circle_body5};
//	BODY* result;
//	api_skin_wires(7,bodies,result);
//	//BODY* cone;
//	//api_solid_cylinder_cone(SPAposition (0,0,13),SPAposition (0,0,25),5,5,0,NULL,cone);
//	ENTITY_LIST pModelEntityList;
//	 pModelEntityList.init();
//	 pModelEntityList.add(result);
//	  //pModelEntityList.add(cone);
//	SaveSatFile("C:\\Users\\Jonathan_Lewis\\Desktop\\testCone.sat",pModelEntityList);
//}

//void test_sphere(){	
//	EDGE* circle_edge,*circle_edge1,*circle_edge2,*circle_edge3,*circle_edge4;
//	BODY* circle_body,* circle_body1,* circle_body2,* circle_body3,* circle_body4;
//	//APOINT* A = new APOINT(0,0,5);
//	//APOINT* B = new APOINT(0,0,-5);
//	//APOINT* a,*b;
//	SPAposition A(0,0,5),B(0,0,-5);
//	//EDGE* point,*point1;
//	BODY* point_body,*point_body1;
//	SPAposition pts[1] = {A};
//	SPAposition pts1[1] = {B};
//	//VERTEX* AA = new VERTEX(A); 
//	//VERTEX* BB = new VERTEX(B); 
//	//api_solid_block(SPAposition(0,0,5), SPAposition(0,0,5),point_body);
//	//api_solid_block(SPAposition(0,0,-5), SPAposition(0,0,-5),point_body1);
//	api_build_wire (NULL, FALSE, 1, pts,NULL, point_body, NULL);
//	api_build_wire (NULL, FALSE, 1, pts1,NULL, point_body1, NULL);//构建点实体的方法
//	//api_create_point(A,a,NULL);
//
//	//api_curve_line(A, A, point,  NULL ) ;
//
//	//api_curve_arc(A,SPAresabs,0,2*3.141592654,point);
//	//api_make_ewire(1,&point,point_body);
//
//	//api_curve_arc(B,SPAresabs,0,2*3.141592654,point1);
//	//api_curve_line(B,B, point,  NULL ) ;
//	//api_make_ewire(1,&point1,point_body1);
//
//	SPAposition C(0,0,3),D(0,0,4),E(0,0,-3),F(0,0,-4),G(0,0,0);
//
//	api_curve_arc(C,4,0,2*3.141592654,circle_edge);
//	api_make_ewire(1,&circle_edge,circle_body);
//
//	api_curve_arc(D,3,0,2*3.141592654,circle_edge1);
//	api_make_ewire(1,&circle_edge1,circle_body1);
//
//	api_curve_arc(E,4,0,2*3.141592654,circle_edge2);
//	api_make_ewire(1,&circle_edge2,circle_body2);
//
//	api_curve_arc(F,3,0,2*3.141592654,circle_edge3);
//	api_make_ewire(1,&circle_edge3,circle_body3);
//
//	api_curve_arc(G,5,0,2*3.141592654,circle_edge4);
//	api_make_ewire(1,&circle_edge4,circle_body4);
//
//	BODY* bodies[7] = {point_body,circle_body1,circle_body,circle_body4,circle_body2,circle_body3,point_body1};
//	BODY* result;
//	api_skin_wires(7,bodies,result);
//
//	ENTITY_LIST pModelEntityList;
//	 pModelEntityList.init();
//	 pModelEntityList.add(result);
//	SaveSatFile("C:\\Users\\Jonathan_Lewis\\Desktop\\testSphere.sat",pModelEntityList);
//}
//void test_sphere1(){	
//	EDGE* circle_edge,*circle_edge1,*circle_edge2,*circle_edge3,*circle_edge4;
//	BODY* circle_body,* circle_body1,* circle_body2,* circle_body3,* circle_body4;
//	//APOINT* A = new APOINT(0,0,5);
//	//APOINT* B = new APOINT(0,0,-5);
//	//APOINT* a,*b;
//	SPAposition A(0,0,5),B(0,0,-5);
//	//EDGE* point,*point1;
//	BODY* point_body,*point_body1;
//	SPAposition pts[1] = {A};
//	SPAposition pts1[1] = {B};
//	//VERTEX* AA = new VERTEX(A); 
//	//VERTEX* BB = new VERTEX(B); 
//	//api_solid_block(SPAposition(0,0,5), SPAposition(0,0,5),point_body);
//	//api_solid_block(SPAposition(0,0,-5), SPAposition(0,0,-5),point_body1);
//	api_build_wire (NULL, FALSE, 1, pts,NULL, point_body, NULL);
//	api_build_wire (NULL, FALSE, 1, pts1,NULL, point_body1, NULL);//构建点实体的方法
//	//api_create_point(A,a,NULL);
//
//	//api_curve_line(A, A, point,  NULL ) ;
//
//	//api_curve_arc(A,SPAresabs,0,2*3.141592654,point);
//	//api_make_ewire(1,&point,point_body);
//
//	//api_curve_arc(B,SPAresabs,0,2*3.141592654,point1);
//	//api_curve_line(B,B, point,  NULL ) ;
//	//api_make_ewire(1,&point1,point_body1);
//
//	SPAposition C(0,0,3),D(0,0,4),E(0,0,-3),F(0,0,-4),G(0,0,0);
//
//	api_curve_arc(C,4,0,2*3.141592654,circle_edge);
//	api_make_ewire(1,&circle_edge,circle_body);
//
//	api_curve_arc(D,3,0,2*3.141592654,circle_edge1);
//	api_make_ewire(1,&circle_edge1,circle_body1);
//
//	api_curve_arc(E,4,0,2*3.141592654,circle_edge2);
//	api_make_ewire(1,&circle_edge2,circle_body2);
//
//	api_curve_arc(F,3,0,2*3.141592654,circle_edge3);
//	api_make_ewire(1,&circle_edge3,circle_body3);
//
//	api_curve_arc(G,5,0,2*3.141592654,circle_edge4);
//	api_make_ewire(1,&circle_edge4,circle_body4);
//
//BODY* bodies[7] = {point_body,circle_body1,circle_body,circle_body4,circle_body2,circle_body3,point_body1};
//	//BODY* result;
//	//api_skin_wires(7,bodies,result);
//
//	ENTITY_LIST pModelEntityList;
//	 pModelEntityList.init();
//	for(int i=0;i<7;++i)
//		 pModelEntityList.add(bodies[i]);
//	SaveSatFile("C:\\Users\\Jonathan_Lewis\\Desktop\\testSphere1.sat",pModelEntityList);
//}

//void test_cover(){
//	/*ENTITY_LIST* list1 = new ENTITY_LIST;
//	ENTITY_LIST *list2 = new ENTITY_LIST;
//	ENTITY_LIST pModelEntityList;
//	 pModelEntityList.init();
//	 list1->add(circle_edge);
//	list2->add(circle_edge1);
//	ENTITY_LIST * array_circuits[2] = {list1,list2};
//	api_cover_circuits(2,array_circuits,*(surface *)NULL,pModelEntityList,NULL);*/
//	 
//	
//	/* pModelEntityList.add(circle_body);
//	 pModelEntityList.add(circle_body1);*/
//	//SaveSatFile("C:\\Users\\Jonathan_Lewis\\Desktop\\testCover.sat",pModelEntityList);
//}

