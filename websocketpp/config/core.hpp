/*
 * Copyright (c) 2012, Peter Thorson. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the WebSocket++ Project nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 * ARE DISCLAIMED. IN NO EVENT SHALL PETER THORSON BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */

#ifndef WEBSOCKETPP_CONFIG_CORE_HPP
#define WEBSOCKETPP_CONFIG_CORE_HPP

// Concurrency
#include <websocketpp/concurrency/basic.hpp>

// Transport
#include <websocketpp/transport/iostream/endpoint.hpp>

// HTTP
#include <websocketpp/http/request.hpp>
#include <websocketpp/http/response.hpp>

// Messages
#include <websocketpp/message_buffer/message.hpp>
#include <websocketpp/message_buffer/alloc.hpp>

// Loggers
#include <websocketpp/logger/basic.hpp>

// Extensions
#include <websocketpp/extensions/permessage_compress/disabled.hpp>

namespace websocketpp {
namespace config {

struct core {
    // Concurrency policy
    typedef websocketpp::concurrency::basic concurrency_type;
	
    // Transport Policy
    typedef websocketpp::transport::iostream::endpoint<concurrency_type> 
        transport_type;

    // HTTP Parser Policies
    typedef http::parser::request request_type;
    typedef http::parser::response response_type;

    // Message Policies
    typedef message_buffer::message<message_buffer::alloc::con_msg_manager> 
		message_type;
    typedef message_buffer::alloc::con_msg_manager<message_type> 
		con_msg_manager_type;
    typedef message_buffer::alloc::endpoint_msg_manager<con_msg_manager_type> 
		endpoint_msg_manager_type;
	
	/// Logging policies
	typedef websocketpp::logger::basic<concurrency_type,
	    websocketpp::logger::error_names> elog_type;
	typedef websocketpp::logger::basic<concurrency_type,
	    websocketpp::logger::access_names> alog_type;
	
    /// 
	static const size_t connection_read_buffer_size = 32;
    
    /// Drop connections immediately on protocol error. 
    /** 
     * Drop connections on protocol error rather than sending a close frame.
     * Off by default. This may result in legit messages near the error being
     * dropped as well. It may free up resources otherwise spent dealing with 
     * misbehaving clients.
     */
    static const bool drop_on_protocol_error = false;
    
    /// Suppresses the return of detailed connection close information
    /**
     * Silence close suppresses the return of detailed connection close
     * information during the closing handshake. This information is useful
     * for debugging and presenting useful errors to end users but may be
     * undesirable for security reasons in some production environments.
     * Close reasons could be used by an attacker to confirm that the endpoint
     * is out of resources or be used to identify the WebSocket implimentation
     * in use.
     *
     * Note: this will suppress *all* close codes, including those explicitly
     * sent by local applications.
     */
    static const bool silent_close = false;
    
    /// Global flag for enabling/disabling extensions
    static const bool enable_extensions = true;
    
    /// Extension specific settings:

    /// permessage_compress extension
    struct permessage_compress_config {
        typedef core::request_type request_type;
        
        /// If the remote endpoint requests that we reset the compression
        /// context after each message should we honor the request?
        static const bool allow_disabling_context_takeover = true;
        
        /// If the remote endpoint requests that we reduce the size of the
        /// LZ77 sliding window size this is the lowest value that will be
        /// allowed. Values range from 8 to 15. A value of 8 means we will
        /// allow any possible window size. A value of 15 means do not allow
        /// negotiation of the window size (ie require the default).
        static const uint8_t minimum_outgoing_window_bits = 8;
    };

    typedef websocketpp::extensions::permessage_compress::disabled
        <permessage_compress_config> permessage_compress_type;

    /// Autonegotiate permessage-compress
    /**
     * Automatically enables the permessage-compress extension. 
     *
     * For clients this results in a permessage-compress extension request being
     * sent with every request rather than requiring it to be requested manually
     * 
     * For servers this results in accepting the first set of extension settings
     * requested by the client that we understand being used. The alternative is
     * requiring the extension to be manually negotiated in `validate`. With 
     * auto-negotiate on, you may still override the auto-negotiate manually if
     * needed.
     */
    //static const bool autonegotiate_compression = false;
};

} // namespace config
} // namespace websocketpp

#endif // WEBSOCKETPP_CONFIG_CORE_HPP