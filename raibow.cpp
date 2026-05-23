#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main() {
    // Create an SVG file with Spiderman illustration
    ofstream svgFile("spiderman.svg");
    
    if (!svgFile.is_open()) {
        cerr << "Error: Could not create SVG file!" << endl;
        return 1;
    }
    
    // Write SVG header and content
    svgFile << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl;
    svgFile << "<svg viewBox=\"0 0 680 800\" width=\"100%\" xmlns=\"http://www.w3.org/2000/svg\" role=\"img\">" << endl;
    svgFile << "  <title>Spider-Man illustration</title>" << endl;
    svgFile << "  <desc>A stylized illustration of Spider-Man in his iconic red and blue suit</desc>" << endl;
    
    svgFile << endl << "  <!-- Web pattern background -->" << endl;
    svgFile << "  <g stroke=\"#ddd\" stroke-width=\"0.5\" fill=\"none\">" << endl;
    
    // Generate vertical web lines
    for (int x = 100; x <= 620; x += 40) {
        svgFile << "    <line x1=\"" << x << "\" y1=\"0\" x2=\"" << x << "\" y2=\"800\"/>" << endl;
    }
    
    // Generate horizontal web lines
    for (int y = 50; y <= 750; y += 50) {
        svgFile << "    <line x1=\"0\" y1=\"" << y << "\" x2=\"680\" y2=\"" << y << "\"/>" << endl;
    }
    
    svgFile << "  </g>" << endl << endl;
    
    // Spider-Man figure - Torso (red suit)
    svgFile << "  <!-- Spider-Man figure -->" << endl;
    svgFile << "  <ellipse cx=\"340\" cy=\"300\" rx=\"80\" ry=\"110\" fill=\"#E63946\"/>" << endl;
    
    // Blue side panels
    svgFile << "  <rect x=\"240\" y=\"250\" width=\"40\" height=\"150\" fill=\"#1D3557\"/>" << endl;
    svgFile << "  <rect x=\"400\" y=\"250\" width=\"40\" height=\"150\" fill=\"#1D3557\"/>" << endl;
    
    // Spider emblem on chest
    svgFile << "  <g transform=\"translate(340, 280)\">" << endl;
    svgFile << "    <circle cx=\"0\" cy=\"0\" r=\"12\" fill=\"#1D3557\"/>" << endl;
    svgFile << "    <line x1=\"-15\" y1=\"-8\" x2=\"-28\" y2=\"-15\" stroke=\"#1D3557\" stroke-width=\"2\" stroke-linecap=\"round\"/>" << endl;
    svgFile << "    <line x1=\"-15\" y1=\"8\" x2=\"-28\" y2=\"15\" stroke=\"#1D3557\" stroke-width=\"2\" stroke-linecap=\"round\"/>" << endl;
    svgFile << "    <line x1=\"15\" y1=\"-8\" x2=\"28\" y2=\"-15\" stroke=\"#1D3557\" stroke-width=\"2\" stroke-linecap=\"round\"/>" << endl;
    svgFile << "    <line x1=\"15\" y1=\"8\" x2=\"28\" y2=\"15\" stroke=\"#1D3557\" stroke-width=\"2\" stroke-linecap=\"round\"/>" << endl;
    svgFile << "  </g>" << endl << endl;
    
    // Head
    svgFile << "  <circle cx=\"340\" cy=\"150\" r=\"50\" fill=\"#E63946\"/>" << endl;
    
    // Eye mask area
    svgFile << "  <ellipse cx=\"320\" cy=\"140\" rx=\"15\" ry=\"20\" fill=\"#1D3557\"/>" << endl;
    svgFile << "  <ellipse cx=\"360\" cy=\"140\" rx=\"15\" ry=\"20\" fill=\"#1D3557\"/>" << endl;
    
    // Eyes (white)
    svgFile << "  <ellipse cx=\"318\" cy=\"138\" rx=\"8\" ry=\"12\" fill=\"white\"/>" << endl;
    svgFile << "  <ellipse cx=\"362\" cy=\"138\" rx=\"8\" ry=\"12\" fill=\"white\"/>" << endl;
    
    // Eye pupils (black)
    svgFile << "  <circle cx=\"318\" cy=\"140\" r=\"5\" fill=\"black\"/>" << endl;
    svgFile << "  <circle cx=\"362\" cy=\"140\" r=\"5\" fill=\"black\"/>" << endl;
    
    // Mouth line
    svgFile << "  <path d=\"M 330 165 Q 340 172 350 165\" stroke=\"#1D3557\" stroke-width=\"2\" fill=\"none\" stroke-linecap=\"round\"/>" << endl << endl;
    
    // Left arm
    svgFile << "  <g>" << endl;
    svgFile << "    <rect x=\"150\" y=\"280\" width=\"35\" height=\"80\" rx=\"17\" fill=\"#E63946\" transform=\"rotate(-35 170 280)\"/>" << endl;
    svgFile << "    <rect x=\"115\" y=\"340\" width=\"30\" height=\"100\" rx=\"15\" fill=\"#E63946\" transform=\"rotate(-50 135 340)\"/>" << endl;
    svgFile << "    <circle cx=\"80\" cy=\"410\" r=\"18\" fill=\"#E8A76B\"/>" << endl;
    svgFile << "  </g>" << endl;
    
    // Right arm
    svgFile << "  <g>" << endl;
    svgFile << "    <rect x=\"495\" y=\"280\" width=\"35\" height=\"80\" rx=\"17\" fill=\"#E63946\" transform=\"rotate(35 515 280)\"/>" << endl;
    svgFile << "    <rect x=\"535\" y=\"340\" width=\"30\" height=\"100\" rx=\"15\" fill=\"#E63946\" transform=\"rotate(50 545 340)\"/>" << endl;
    svgFile << "    <circle cx=\"600\" cy=\"410\" r=\"18\" fill=\"#E8A76B\"/>" << endl;
    svgFile << "  </g>" << endl << endl;
    
    // Left leg
    svgFile << "  <g>" << endl;
    svgFile << "    <rect x=\"270\" y=\"390\" width=\"40\" height=\"120\" rx=\"20\" fill=\"#1D3557\"/>" << endl;
    svgFile << "    <rect x=\"275\" y=\"500\" width=\"30\" height=\"100\" rx=\"15\" fill=\"#E63946\"/>" << endl;
    svgFile << "    <ellipse cx=\"290\" cy=\"620\" rx=\"22\" ry=\"18\" fill=\"#1D3557\"/>" << endl;
    svgFile << "  </g>" << endl;
    
    // Right leg
    svgFile << "  <g>" << endl;
    svgFile << "    <rect x=\"370\" y=\"390\" width=\"40\" height=\"120\" rx=\"20\" fill=\"#1D3557\"/>" << endl;
    svgFile << "    <rect x=\"375\" y=\"500\" width=\"30\" height=\"100\" rx=\"15\" fill=\"#E63946\"/>" << endl;
    svgFile << "    <ellipse cx=\"390\" cy=\"620\" rx=\"22\" ry=\"18\" fill=\"#1D3557\"/>" << endl;
    svgFile << "  </g>" << endl << endl;
    
    // Web strands from hands
    svgFile << "  <g stroke=\"#4ECDC4\" stroke-width=\"1.5\" fill=\"none\" opacity=\"0.8\">" << endl;
    svgFile << "    <path d=\"M 80 410 L 60 350 L 40 360 L 20 340\"/>" << endl;
    svgFile << "    <path d=\"M 80 410 L 70 480 L 50 500 L 30 480\"/>" << endl;
    svgFile << "    <path d=\"M 600 410 L 620 350 L 640 360 L 660 340\"/>" << endl;
    svgFile << "    <path d=\"M 600 410 L 610 480 L 630 500 L 650 480\"/>" << endl;
    svgFile << "  </g>" << endl << endl;
    
    // Title
    svgFile << "  <text x=\"340\" y=\"750\" text-anchor=\"middle\" font-size=\"28\" font-weight=\"bold\" fill=\"#E63946\" font-family=\"Arial, sans-serif\">" << endl;
    svgFile << "    SPIDER-MAN" << endl;
    svgFile << "  </text>" << endl;
    svgFile << "  <text x=\"340\" y=\"785\" text-anchor=\"middle\" font-size=\"14\" fill=\"#1D3557\" font-family=\"Arial, sans-serif\">" << endl;
    svgFile << "    Your Friendly Neighborhood Web-Slinger" << endl;
    svgFile << "  </text>" << endl;
    
    // Close SVG
    svgFile << "</svg>" << endl;
    
    svgFile.close();
    
    cout << "✓ Spiderman SVG image created successfully!" << endl;
    cout << "✓ File saved as: spiderman.svg" << endl;
    cout << endl;
    cout << "To view the image:" << endl;
    cout << "  - Open spiderman.svg in any web browser" << endl;
    cout << "  - Or use an image viewer that supports SVG files" << endl;
    
    return 0;
}