#include <httplib.h>

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace {

std::string env(const char *name, const std::string &fallback) {
    const char *value = std::getenv(name);
    return value ? value : fallback;
}

std::string readFile(const std::filesystem::path &path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Could not read template: " + path.string());
    }
    std::ostringstream contents;
    contents << file.rdbuf();
    return contents.str();
}

std::string renderAcademy() {
    const auto templates = std::filesystem::path(env("TEMPLATES_DIR", "templates"));
    std::string page = readFile(templates / "academy.html");
    const auto contentStart = page.find("{% block content %}");
    const auto contentEnd = page.rfind("{% endblock %}");
    if (contentStart == std::string::npos || contentEnd == std::string::npos ||
        contentEnd < contentStart) {
        throw std::runtime_error("Academy page has an invalid content block");
    }

    const auto blockStart = contentStart + std::string("{% block content %}").size();
    const auto content = page.substr(blockStart, contentEnd - blockStart);
    std::string base = readFile(templates / "base.html");
    const std::string block = "{% block content %}{% endblock %}";
    const auto insertionPoint = base.find(block);
    if (insertionPoint == std::string::npos) {
        throw std::runtime_error("Base page has no content block");
    }
    base.replace(insertionPoint, block.size(), content);
    return base;
}

void setSecurityHeaders(httplib::Response &response) {
    response.set_header("X-Content-Type-Options", "nosniff");
    response.set_header("X-Frame-Options", "DENY");
    response.set_header("Referrer-Policy", "strict-origin-when-cross-origin");
    response.set_header("Content-Security-Policy",
                        "default-src 'self'; script-src 'self' 'unsafe-inline'; "
                        "style-src 'self' 'unsafe-inline'; img-src 'self' data:; "
                        "media-src 'self' blob: data:; connect-src 'self' blob:; "
                        "base-uri 'self'; frame-ancestors 'none'");
    response.set_header("Cache-Control", "no-store");
}

void sendAcademy(httplib::Response &response) {
    setSecurityHeaders(response);
    try {
        response.set_content(renderAcademy(), "text/html; charset=utf-8");
    } catch (const std::exception &error) {
        std::cerr << "Academy page rendering failed: " << error.what() << '\n';
        response.status = 500;
        response.set_content("Nu s-a putut încărca Academia.", "text/plain; charset=utf-8");
    }
}

} // namespace

int main() {
    try {
        httplib::Server server;
        server.Get("/", [](const httplib::Request &, httplib::Response &response) {
            sendAcademy(response);
        });
        server.Get("/academy", [](const httplib::Request &, httplib::Response &response) {
            sendAcademy(response);
        });
        server.Get("/healthz", [](const httplib::Request &, httplib::Response &response) {
            setSecurityHeaders(response);
            response.set_content("ok", "text/plain; charset=utf-8");
        });
        server.Get("/favicon.ico", [](const httplib::Request &, httplib::Response &response) {
            setSecurityHeaders(response);
            response.status = 204;
        });
        server.set_error_handler([](const httplib::Request &, httplib::Response &response) {
            setSecurityHeaders(response);
            response.set_content("Pagina nu a fost găsită.", "text/plain; charset=utf-8");
        });

        const int port = std::stoi(env("PORT", "8080"));
        const auto host = env("HOST", "127.0.0.1");
        std::cout << "NeonMix Academy listening on " << host << ':' << port << '\n';
        if (!server.listen(host, port)) {
            throw std::runtime_error("Could not start HTTP server");
        }
        return 0;
    } catch (const std::exception &error) {
        std::cerr << "NeonMix startup failed: " << error.what() << '\n';
        return 1;
    }
}
