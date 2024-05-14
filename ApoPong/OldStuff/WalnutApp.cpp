#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/Input/Input.h"
#include "Pong.h"

class PongLayer : public Walnut::Layer
{
public:
	PongLayer() {}

	virtual void OnUpdate(float ts) override
	{
		// if (Walnut::Input::IsKeyDown(Walnut::KeyCode::Space))
		// 	mPong.StartGame(Pong::Modifier::AdditivePointSystem);

		mPong.OnUpdate(ts);
		mPong.OnRender();
	}

	// Takes the final image and renders it to the window
	virtual void OnUIRender() override
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::Begin("Viewport");

		std::shared_ptr<Walnut::Image> image = mPong.GetImage();
		if (image)
			ImGui::Image(image->GetDescriptorSet(), ImVec2((float)image->GetWidth(), (float)image->GetHeight()));

		ImGui::End();

		uint32_t left_score = mPong.GetLeftScore();
		uint32_t right_score = mPong.GetRightScore();

		ImGui::Begin("Led Line");

		Pong::Modifier mod = mPong.GetModifier();
		if (EnumHasAnyFlags(mod, Pong::Modifier::AdditivePointSystem))
		{
			auto* draw_list = ImGui::GetWindowDrawList();
			ImVec2 window_pos = ImGui::GetWindowPos();
			uint32_t points = mPong.GetBounces();

			ImVec2 min(window_pos.x + 10, window_pos.y + 40);
			ImVec2 max(window_pos.x + 20, window_pos.y + 50);
			for (int i = 31; i >= 0; i--)
			{
				if (i < points)
					draw_list->AddRectFilled(min, max, 0xffffffff);
				else
					draw_list->AddRectFilled(min, max, 0xff000000);

				min.x += 20;
				max.x += 20;
			}
		}
		else
		{
			auto* draw_list = ImGui::GetWindowDrawList();
			ImVec2 window_pos = ImGui::GetWindowPos();

			ImVec2 min(window_pos.x + 10, window_pos.y + 40);
			ImVec2 max(window_pos.x + 20, window_pos.y + 50);

			for (int i = 0; i < 16; i++)
			{
				if (i < left_score)
					draw_list->AddRectFilled(min, max, 0xffffffff);
				else
					draw_list->AddRectFilled(min, max, 0xff000000);

				min.x += 20;
				max.x += 20;
			}

			for (int i = 15; i >= 0; i--)
			{
				if (i < right_score)
					draw_list->AddRectFilled(min, max, 0xffffffff);
				else
					draw_list->AddRectFilled(min, max, 0xff000000);

				min.x += 20;
				max.x += 20;
			}
		}

		ImGui::End();

		uint32_t led_color = 0xff000000;
		if (EnumHasAnyFlags(mod, Pong::Modifier::AdditivePointSystem))
		{
			led_color += 0xff8b3d48;
		}

		if (EnumHasAnyFlags(mod, Pong::Modifier::AcceleratingBall))
		{
			led_color += 0xff00a5ff;
		}

		ImGui::Begin("RGB Led Diodes");
		{
			auto* draw_list = ImGui::GetWindowDrawList();
			ImVec2 window_pos = ImGui::GetWindowPos();

			ImVec2 min(window_pos.x + 10, window_pos.y + 40);
			ImVec2 max(window_pos.x + 40, window_pos.y + 70);
			draw_list->AddRectFilled(min, max, led_color);

			min.x += 50;
			max.x += 50;
			draw_list->AddRectFilled(min, max, led_color);
		}
		ImGui::End();

		ImGui::PopStyleVar();

		mPong.GetMenu()->OnRender();
	}

private:
	Pong::Pong mPong;
};

/*
* Vytv���m tady okno o velikosti toho lcd displaye (nelze resiznout) a v n�m se samo�ejm� renderurje na�e hra. 
* Okolo ti taky vysko�ila dal�� okna. To je gui knihovna ImGui. 'Pokusil' jsem se t�m emulovat UI v na�� h�e, 
* jeliko� renderovat text jako takov� tu nejde.
* 
* Jedno je Main Menu a m��e� pomoc� Enter zvolit jestli chce� hr�t nebo si vybrat modifik�tory.
* ! Modifik�tory vybere� pomoc� Enter, ale tak jak to funguje, tak ta aplikace se jen kouk� jestli je Enter zm��klej, tak�e 
* se ten mod m��e zapnout, vypnout... !
* Escape aby ses vr�til do hlavn�ho menu
* 
* Play za�ne novou hru. Spacebar pro vypu�t�n� m��e. W, S - pohyb �erven�ho hr��e, Up, Down - pohyb modr�ho hr��e.
* 
* 'Emuloval' jsem i ty periferije. RGB led m�n� barvu podle modifik�tor�
*/

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "Pong - Graphics";
	spec.Width = 480;
	spec.Height = 320;
	spec.WindowResizeable = false;

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<PongLayer>();
	return app;
}
