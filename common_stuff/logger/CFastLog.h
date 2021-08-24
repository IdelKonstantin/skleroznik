#ifndef FAST_LOG_HEADER_GUARD
#define FAST_LOG_HEADER_GUARD

#include <cstdint>
#include <memory>
#include <string>
#include <iosfwd>
#include <type_traits>

namespace fastlog
{
    const std::string LOG_INFO_TAG = "ИНФОРМАЦИЯ";
    const std::string LOG_WARNING_TAG = "ПРЕДУПРЕЖДЕНИЕ";
    const std::string LOG_ERROR_TAG = "ОШИБКА";

    enum class LogLevel : uint8_t { INFO, WARN, CRIT };
    
    class CFastLogLine
    {
    public:
	CFastLogLine(LogLevel level, char const * file, char const * function, uint32_t line);
	~CFastLogLine();

	CFastLogLine(CFastLogLine &&) = default;
	CFastLogLine& operator=(CFastLogLine &&) = default;

	void stringify(std::ostream & os);

	CFastLogLine& operator<<(char arg);
	CFastLogLine& operator<<(int32_t arg);
	CFastLogLine& operator<<(uint32_t arg);
	CFastLogLine& operator<<(int64_t arg);
	CFastLogLine& operator<<(uint64_t arg);
	CFastLogLine& operator<<(double arg);
	CFastLogLine& operator<<(std::string const & arg);

	template < size_t N >
	CFastLogLine& operator<<(const char (&arg)[N])
	{
	    encode(string_literal_t(arg));
	    return *this;
	}

	template < typename Arg >
	typename std::enable_if < std::is_same < Arg, char const * >::value, CFastLogLine& >::type
	operator<<(Arg const & arg)
	{
	    encode(arg);
	    return *this;
	}

	template < typename Arg >
	typename std::enable_if < std::is_same < Arg, char * >::value, CFastLogLine& >::type
	operator<<(Arg const & arg)
	{
	    encode(arg);
	    return *this;
	}

	struct string_literal_t
	{
	    explicit string_literal_t(char const * s) : m_s(s) {}
	    char const * m_s;
	};

    private:	
	char * buffer();

	template < typename Arg >
	void encode(Arg arg);

	template < typename Arg >
	void encode(Arg arg, uint8_t type_id);

	void encode(char * arg);
	void encode(char const * arg);
	void encode(string_literal_t arg);
	void encode_c_string(char const * arg, size_t length);
	void resize_buffer_if_needed(size_t additional_bytes);
	void stringify(std::ostream & os, char * start, char const * const end);

    private:
	size_t m_bytes_used;
	size_t m_buffer_size;
	std::unique_ptr < char [] > m_heap_buffer;
	char m_stack_buffer[256 - 2 * sizeof(size_t) - sizeof(decltype(m_heap_buffer)) - 8 /* Reserved */];
    };
    
    struct CFastLog
    {
	/*
	 * Возможно стоит поменять на operator +=
	 */
	bool operator==(CFastLogLine &);
    };

    void set_log_level(LogLevel level);
    
    bool is_logged(LogLevel level);


    /*
     * Негарантированное логирование. Используется кольцевой буфер для хранения строк.
     * При заполнении буфера наиболее старые строки в логе будут замещаться новыми.
     * Это позволяет избежать блокировки записи в случае, когда достигнут максимальный размер журнала
     * ring_buffer_size_mb - размер буфера в мегабайтах
     * Поскольку размер каждой строки в журнале равен 256  байт,
     * ring_buffer_size = ring_buffer_size_mb * 1024 * 1024 / 256
     */
    struct NonGuaranteedLogger
    {
	NonGuaranteedLogger(uint32_t ring_buffer_size_mb_) : ring_buffer_size_mb(ring_buffer_size_mb_) {}
	uint32_t ring_buffer_size_mb;
    };

    /*
     * Гарантированное логирование, без потерь старых записей.
     */
    struct GuaranteedLogger
    {
    };
    
    /*
     * Функция initialize() должна быть вызвана до любого обращения к журналу.
     * log_directory - папка хранения файла журнала. Например - "/tmp/"
     * log_file_name - базовая часть имени файла журнала. Например - "fastlog"
     * При этом будут создаваться файлы в формате -
     * /tmp/fastlog.1.txt
     * /tmp/fastlog.2.txt
     * и т.д.
     * log_file_roll_size_mb - размер в мегабайтах, при котором происходит переход к новому файлу журнала.
     */
    void initialize(GuaranteedLogger gl, std::string const & log_directory, std::string const & log_file_name, uint32_t log_file_roll_size_mb);
    void initialize(NonGuaranteedLogger ngl, std::string const & log_directory, std::string const & log_file_name, uint32_t log_file_roll_size_mb);

} // namespace fastlog

#define FAST_LOG(LEVEL) fastlog::CFastLog() == fastlog::CFastLogLine(LEVEL, __FILE__, __func__, __LINE__)
#define LOG_INFO fastlog::is_logged(fastlog::LogLevel::INFO) && FAST_LOG(fastlog::LogLevel::INFO)
#define LOG_WARN fastlog::is_logged(fastlog::LogLevel::WARN) && FAST_LOG(fastlog::LogLevel::WARN)
#define LOG_CRIT fastlog::is_logged(fastlog::LogLevel::CRIT) && FAST_LOG(fastlog::LogLevel::CRIT)

#endif /* FAST_LOG_HEADER_GUARD */

