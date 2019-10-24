# include <Siv3D.hpp> // OpenSiv3D v0.4.1
using namespace std;

void draw_frames(Size box_size){
	Line(0,box_size.y, Window::ClientWidth(), box_size.y).draw();
	Line(0, box_size.y*2, Window::ClientWidth(), box_size.y*2).draw();
	Line(box_size.x,0 ,box_size.x ,Window::ClientHeight()).draw();
	Line(box_size.x*2, 0, box_size.x*2, Window::ClientHeight()).draw();
}

void draw_field(Grid<int> field,Size box_size){
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			switch (field[i][j]) {
			case 1:
				Circle(box_size / 2 + Point(box_size.x * i, box_size.y * j),
					Min(box_size.x,box_size.y)/2).drawFrame(10,0);
				break;
			case 2:
				Shape2D::Cross(
					Min(box_size.x, box_size.y) / 2,
					10,
					box_size / 2 + Point(box_size.x * i, box_size.y * j)).draw();
			default:
				break;
			}
		}
	}
}

void Main()
{
	Size box_size;
	Grid<int> field(3,3,0);
	bool is_circle = true, is_finished = false;
	unsigned char who_wins = 0;
	unsigned char finish_cnt=9;
	Point start_point, end_point;
	const Font font(50);
	while (System::Update())
	{
		box_size = Window::ClientSize() / 3;
		draw_frames(box_size);
		if(finish_cnt<=0) is_finished=true;
		
		if (is_finished) {
			draw_field(field, box_size);
			String text;
			switch (who_wins)
			{
			case 1:
					text=U"Circle Wins!";
				break;
			case 2:
					text=U"Cross Win";
				break;
			default:
					text=U"Draw!";
				break;
			}
			font(text).drawAt(Scene::Center(),Palette::Skyblue);
			if(finish_cnt > 0) Line(start_point, end_point).draw(10);
			if (SimpleGUI::Button(U"Restart", Vec2(500, 500)))
			{
				is_circle=true;
				who_wins=0;
				is_finished = false;
				finish_cnt=9;
				start_point=Point();
				end_point=Point();
				field.assign(3, 3, 0);
			}
		}
		else
		{
			if (is_circle) Circle(Cursor::Pos(), 80).drawFrame(10, 0);
			else Shape2D::Cross(80, 10, Cursor::Pos()).draw();
			Point pos = Cursor::Pos();
			if (MouseL.down()) {
				for (int i = 0; i < 3; i++) {
					if (box_size.x * i < pos.x&&pos.x < box_size.x * (i + 1)) {
						for (int j = 0; j < 3; j++) {
							if (box_size.y * j < pos.y && pos.y < box_size.y * (j + 1)&&field[i][j]==0) {
								int target = is_circle ? 1 : 2;
								field[i][j] = target;
								int i1 = (i + 1) % 3, i2 = (i + 2) % 3, j1 = (j + 1) % 3, j2 = (j + 2) % 3;
								if (target == field[i1][j] && target == field[i2][j]) {
									is_finished = true;
									who_wins = target;
									start_point = box_size / 2 + Point(0, box_size.y * j);
									end_point = start_point + Point(box_size.x * 2, 0);
								}
								else if (target == field[i][j1] && target == field[i][j2]) {
									is_finished = true;
									who_wins = target;
									start_point = box_size / 2 + Point(box_size.x*i,0);
									end_point = start_point + Point(0,box_size.y*2);
								}
								else if (i == j && target == field[i1][j1] && target == field[i2][j2]) {
									is_finished = true;
									who_wins = target;
									start_point = box_size / 2;
									end_point = start_point + box_size * 2;
								}
								else if (j == -i + 2 && target == field[i1][j2] && target == field[i2][j1]) {
									is_finished = true;
									who_wins = target;
									start_point = box_size / 2 + Point(0, box_size.y * 2);
									end_point = start_point + Point(box_size.x, -box_size.y) * 2;
								}
								is_circle = !is_circle;
								finish_cnt--;
								break;
							}
						}
						break;
					}
				}

			}
			draw_field(field, box_size);
		}
		
	}
}

