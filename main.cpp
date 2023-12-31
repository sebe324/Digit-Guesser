#include <iostream>
#include <SFML/Graphics.hpp>
#include "GUI/DrawingBoard.h"
#include "GUI/Button.h"
#include <string>
#include "NeuralNetwork/NeuralNetwork.h"
#include <ctime>
#include "GUI/ProgressBar.h"

Button createButton(const std::string& text, unsigned charSize, const sf::Vector2f& position, const sf::Vector2f& size, const sf::Font& font)
{
	Button b(text, charSize, sf::Color::White, position, size, sf::Color::Black, font);
	b.hoverBodyColor = sf::Color(50, 50, 50);
	b.body.setOutlineColor(sf::Color::White);
	b.body.setOutlineThickness(2);

	return b;
}

int main()
{
	srand(1234567); //funny

	const int windowWidth = 1200;
	const int windowHeight = 800;
	sf::ContextSettings settings(0, 0, 4);
	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Number Guesser", sf::Style::Close | sf::Style::Resize, settings);
	window.setFramerateLimit(60);
	DrawingBoard drawingBoard(28, 28, 15, 100, 200);
	drawingBoard.updateCells();
	sf::Vector2f mousePos;

	sf::Font font;


	if (!font.loadFromFile("font.ttf")) return -1;



	sf::Text title("Digit Guesser", font, 40);
	title.setFillColor(sf::Color::White);
	title.setPosition(sf::Vector2f((windowWidth - title.getGlobalBounds().width) / 2, 50));

	sf::Text brushSizeLabel("Brush size: ", font, 30);
	brushSizeLabel.setFillColor(sf::Color::White);
	brushSizeLabel.setPosition(sf::Vector2f(100, 650));

	sf::Text brushSizeValue(std::to_string(drawingBoard.brushSize), font, 30);
	brushSizeValue.setFillColor(sf::Color::White);
	brushSizeValue.setPosition(sf::Vector2f(420, 650));

	Button buttonBrushSizeInc = createButton("+", 40, sf::Vector2f(324, 650), sf::Vector2f(50, 50), font);

	Button buttonBrushSizeDec = createButton("-", 40, sf::Vector2f(474, 650), sf::Vector2f(50, 50), font);


	Button buttonStartPerceptron = createButton("START", 35, sf::Vector2f(575, 320), sf::Vector2f(180, 50), font);


	Button buttonClearBoard = createButton("Clear board", 30, sf::Vector2f(220, 133), sf::Vector2f(303, 60), font);


	Button buttonWriteMode = createButton("", 0, sf::Vector2f(157, 133), sf::Vector2f(60, 60), font);
	buttonWriteMode.setSprite("pencil.png", sf::Vector2f(0.1, 0.1));

	Button buttonEraseMode = createButton("", 0, sf::Vector2f(97, 133), sf::Vector2f(60, 60), font);
	buttonEraseMode.setSprite("rubber.png", sf::Vector2f(0.1, 0.1));

	std::vector<ProgressBar> progressBars(10);

	for (int i = 0; i < progressBars.size(); i++) {
		progressBars[i] = ProgressBar(sf::Vector2f(835, 133 + i * 57), sf::Vector2f(200, 50), sf::Color(30, 30, 30), sf::Color::White, 1.0, font);
		progressBars[i].background.setOutlineColor(sf::Color::White);
		progressBars[i].background.setOutlineThickness(2);
		progressBars[i].setLabel(std::to_string(i));
	}
	sf::Clock clock;
	sf::Time deltaTime = sf::seconds(0.016);

	std::vector<unsigned> topology = { 784,100,100,50,10 };
	NeuralNetwork perceptron(topology);
	perceptron.saveWeights(std::vector<std::string>({ "weights1.csv","weights2.csv","weights3.csv","weights4.csv" }));
	perceptron.loadWeights(std::vector<std::string>({ "weights1.csv","weights2.csv","weights3.csv","weights4.csv" }));

	std::vector<std::vector<unsigned char>> data = perceptron.loadTrainingData("LearningBatch\\data", 10000);
	std::vector<std::vector<double>> squishedData(10000, std::vector<double>(784));
	std::vector<std::vector<double>> targetValues = perceptron.getTargetValues("LearningBatch\\labels", 10000);
	//squish the data between 0 and 1
	for (int i = 0; i < 10000; i++) {
		for (int j = 0; j < 784; j++) {
			if (data[i][j] > 0) squishedData[i][j] = 1.0;
			else squishedData[i][j] = 0.0;
		}
	}
	drawingBoard.updateCells();

	std::ofstream file("results.txt");
	file.clear();
	file << "LEARNING BEGINS" << std::endl;
	unsigned iteration = 0;
	std::vector<double> results;
	for (int i = 0; i < 10000; i++) {
		std::cout << "iteration: " << iteration << std::endl;
		file << "iteration: " << iteration++ << std::endl;
		file << "Target values: " << std::endl;
		for (int j = 0; j < 10; j++) file << targetValues[i][j] << " ";
		file << '\n';
		perceptron.feedForward(squishedData[i]);
		perceptron.backpropagate(targetValues[i]);
		perceptron.getResults(results);
		file << "Actual values: " << std::endl;
		for (int j = 0; j < 10; j++) file << results[j]<<" ";
		file << std::endl;
		file << "recent average error: "
			<< perceptron.m_recentAverageError << std::endl;
		std::cout << "recent average error: "
			<< perceptron.m_recentAverageError << std::endl;
		file << "cost : " << perceptron.m_error << std::endl;
		std::cout << "cost : " << perceptron.m_error << std::endl;
	}
	file.close();
	perceptron.saveWeights(std::vector<std::string>({ "weights1.csv","weights2.csv","weights3.csv","weights4.csv" }));
	//perceptron.createLearningBatch("LearningBatch\\data", "LearningBatch\\labels");
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
				case sf::Event::Closed:
					window.close();
				break;
				case sf::Event::MouseMoved:
					mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
					drawingBoard.hover(mousePos);
				break;
			}
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				drawingBoard.click(mousePos);
				if (buttonBrushSizeDec.click(mousePos)) {
					drawingBoard.changeBrushSize(drawingBoard.brushSize - 1);
					brushSizeValue.setString(std::to_string(drawingBoard.brushSize));
				}
				else if(buttonBrushSizeInc.click(mousePos)) {
					drawingBoard.changeBrushSize(drawingBoard.brushSize + 1);
					brushSizeValue.setString(std::to_string(drawingBoard.brushSize));
				}
				else if (buttonWriteMode.click(mousePos)) {
					drawingBoard.changeMode(WRITE);
				}
				else if (buttonEraseMode.click(mousePos)) {
					drawingBoard.changeMode(ERASE);
				}
				else if (buttonClearBoard.click(mousePos)) {
					drawingBoard.clear();
				}
				else if (buttonStartPerceptron.click(mousePos)) {
					perceptron.feedForward(drawingBoard.getValuesFrom0To1());
					std::vector<double> values;
					perceptron.getResults(values);
					for (int i = 0; i < progressBars.size(); i++) progressBars[i].setValue(values[i]);
				}
			}
		}

		buttonBrushSizeDec.update(mousePos, deltaTime);
		buttonBrushSizeInc.update(mousePos, deltaTime);
		buttonStartPerceptron.update(mousePos, deltaTime);
		buttonClearBoard.update(mousePos, deltaTime);
		buttonWriteMode.update(mousePos, deltaTime);
		buttonEraseMode.update(mousePos, deltaTime);

		for (auto& x : progressBars) x.update(deltaTime);
		window.clear();
		window.draw(drawingBoard);
		window.draw(buttonBrushSizeInc);
		window.draw(buttonBrushSizeDec);
		window.draw(buttonStartPerceptron);
		window.draw(buttonClearBoard);
		window.draw(title);
		window.draw(brushSizeLabel);
		window.draw(brushSizeValue);
		window.draw(buttonWriteMode);
		window.draw(buttonEraseMode);

		for (auto& x : progressBars) window.draw(x);
		window.display();
		deltaTime = clock.restart();
	}
	return 0;
}