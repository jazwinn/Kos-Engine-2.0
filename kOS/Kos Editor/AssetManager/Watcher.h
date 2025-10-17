#pragma once
#include "Config/pch.h"
#include "Events/Delegate.h"

enum ACTION
{
	ADDED,
	MODIFIED,
	REMOVED
};


class Watcher {
public:
	using Callback = std::function<void(ACTION, const std::filesystem::path&)>;

	Watcher(const std::string& path, std::chrono::milliseconds delay)
		: m_path(path), m_delay(delay), m_running(false) {
	}

	~Watcher() {
		Stop();
	}

	void Start() {
		m_running = true;
		m_thread = std::thread([this]() { Run(); });
	}

	void Stop() {
		m_running = false;
		if (m_thread.joinable()) {
			m_thread.join();
		}
	}

	void Pause() { m_paused = true; }
	void Resume() { m_paused = false; }
	bool IsPaused() const { return m_paused; }

	template <typename... T>
	void SetIgnoreExtension(T&&... extensions) {
		(m_ignoredExtensions.push_back(std::forward<T>(extensions)), ...);
	}

	void SetIgnoreDirectory(bool boolean) {
		m_IgnoreDirectory = boolean;
	}

	void SetCallback(Callback cb) {
		m_Callback = cb;
	}

	//Delegate<void(const std::filesystem::path&)> delegateisModified;
	//Delegate<void(const std::filesystem::path&)> delegateisAdded;
	//Delegate<void(const std::filesystem::path&)> delegateisRemoved;

private:
	bool m_IgnoreDirectory = true;
	bool m_paused = false;
	std::string m_path;
	std::chrono::milliseconds m_delay;
	std::unordered_map<std::string, std::filesystem::file_time_type> m_files;
	std::thread m_thread;
	std::atomic<bool> m_running;
	std::vector<std::string> m_ignoredExtensions;
	
	Callback m_Callback;


	void Run() {
		while (m_running) {
			if (!m_paused) {
				CheckFiles();
			}
			std::this_thread::sleep_for(m_delay);
		}
	}

	void CheckFiles() {

		auto isIgnored = [&](const std::string& path) -> bool{

			if (m_IgnoreDirectory && std::filesystem::is_directory(path)) {
				return true;
			}


			std::filesystem::path fsPath(path);
			std::string ext = fsPath.extension().string(); // includes the dot, e.g. ".txt"

			return std::find(m_ignoredExtensions.begin(),
				m_ignoredExtensions.end(),
				ext) != m_ignoredExtensions.end();
		};


		// Check for added or modified files
		for (auto& file : std::filesystem::recursive_directory_iterator(m_path)) {
			auto currentFileTime = std::filesystem::last_write_time(file);
			const std::string pathStr = file.path().string();

			if (isIgnored(pathStr))
				continue;

			if (!m_files.contains(pathStr)) {
				m_files[pathStr] = currentFileTime;
				Notify(ADDED, pathStr);
			}
			else if (m_files[pathStr] != currentFileTime) {
				m_files[pathStr] = currentFileTime;
				Notify(MODIFIED, pathStr);
			}
		}

		// Check for removed files
		for (auto it = m_files.begin(); it != m_files.end();) {
			if (!std::filesystem::exists(it->first)) {
				Notify(REMOVED, it->first);
				it = m_files.erase(it);
			}
			else {
				++it;
			}
		}
	}

	void Notify(ACTION action, const std::filesystem::path& filepath) {
		if (m_Callback) {
			m_Callback(action, filepath);
		}
		else {
			std::cout << "File " << action << ": " << filepath << "\n";
		}
	}
};

