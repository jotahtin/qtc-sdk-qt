/****************************************************************************
**
** Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtEnginio module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QCLOUDSERVICES_QCLOUDSERVICES_CONSTANTS_P_H
#define QCLOUDSERVICES_QCLOUDSERVICES_CONSTANTS_P_H

#include <QtCloudServices/qtcloudservices_global.h>
#include <QtCore/qstring.h>

#define FOR_EACH_QTCLOUDSERVICES_STRING(F)\
	F(service_path_objects, "/v1/objects/")\
	\
	F(id, "id") \
	F(objectType, "objectType")\
	F(createdAt, "createdAt")\
	F(creator, "creator")\
	F(updatedAt, "updatedAt")\
	F(updater, "updater")\
	F(results, "results")\
    F(limit, "limit")\
    F(offset, "offset")\
    F(query_q, "q")\
    F(path_separator, "/")\
    \
    F(_synced, "_synced")\
    F(_delete, "delete")\
    F(access, "access")\
    F(access_token, "access_token")\
    F(apiEnginIo, "https://api.engin.io")\
    F(apiRequestId, "apiRequestId")\
    F(complete, "complete")\
    F(count, "count")\
    F(create, "create")\
    F(data, "data")\
    F(empty, "empty")\
    F(event, "event")\
    F(expiringUrl, "expiringUrl")\
    F(file, "file")\
    F(fileName, "fileName")\
    F(files, "files")\
    F(grant_type, "grant_type")\
    F(headers, "headers")\
    F(include, "include")\
    F(incomplete, "incomplete")\
    F(length, "length")\
    F(member, "member")\
    F(members, "members")\
    F(message, "message")\
    F(messageType, "messageType")\
    F(object, "object")\
    F(objectTypes, "objectTypes")\
    F(origin, "origin")\
    F(pageSize, "pageSize")\
    F(password, "password")\
    F(payload, "payload")\
    F(propertyName, "propertyName")\
    F(query, "query")\
    F(search, "search")\
    F(session, "session")\
    F(sessionToken, "sessionToken")\
    F(sort, "sort")\
    F(stagingEnginIo, "https://staging.engin.io")\
    F(status, "status")\
    F(targetFileProperty, "targetFileProperty")\
    F(update, "update")\
    F(url, "url")\
    F(usergroups, "usergroups")\
    F(username, "username")\
    F(users, "users")\
    F(variant, "variant")\
    F(v1_auth_oauth2_token, "/v1/auth/oauth2/token")\
    F(v1_auth_identity, "/v1/auth/identity")\
 
#define FOR_EACH_QTCLOUDSERVICES_BYTEARRAY(F)\
	F(X_Request_Id, "X-Request-Id") \
	F(Host, "Host") \
	F(Accept_Encoding, "Accept-Encoding") \
	F(Accept_Encoding_Any, "*") \
	F(User_Agent, "User-Agent") \
	F(User_Agent_Default, "qtc-sdk-qt/1.0") \
	F(Enginio_Backend_Id, "Enginio-Backend-Id") \
	F(RESTOperationGet, "GET") \
	F(RESTOperationPost, "POST") \
	F(RESTOperationPut, "PUT") \
	F(RESTOperationDelete, "DELETE") \
    F(Authorization, "Authorization")\
    F(Bearer, "Bearer ")\
    \
    F(Requested_object_operation_requires_non_empty_objectType_value, "Requested object operation requires non empty \'objectType\' value")\
    F(Requested_object_acl_operation_requires_non_empty_objectType_value, "Requested object acl operation requires non empty \'objectType\' value")\
    F(Requested_object_acl_operation_requires_non_empty_id_value, "Requested object acl operation requires non empty \'id\' value")\
    F(Download_operation_requires_non_empty_fileId_value, "Download operation requires non empty \'fileId\' value")\
    F(Requested_usergroup_member_operation_requires_non_empty_id_value, "Requested usergroup member operation requires non empty \'id\' value")\
    F(Requested_operation_requires_non_empty_id_value, "Requested operation requires non empty \'id\' value")\
    F(Enginio_Backend_Session, "Enginio-Backend-Session")\
    F(Delete, "DELETE")\
    F(Fulltext_Search_objectTypes_parameter_is_missing_or_it_is_not_an_array, "Fulltext Search: 'objectTypes' parameter is missing or it is not an array")\
    F(Fulltext_Search_search_parameter_missing, "Fulltext Search: 'search' parameter(s) missing")\
    F(Application_octet_stream, "application/octet-stream")\
    F(Application_json, "application/json")\
    F(Application_x_www_form_urlencoded, "application/x-www-form-urlencoded")\
    F(Minus, "-")\
    F(Div, "/")\
    F(EnginioModel_remove_row_is_out_of_range, "EnginioModel::remove: row is out of range")\
    F(EnginioModel_setProperty_row_is_out_of_range, "EnginioModel::setProperty: row is out of range")\
    F(EnginioQmlModel_remove_row_is_out_of_range, "EnginioQmlModel::remove: row is out of range")\
    F(EnginioQmlModel_setProperty_row_is_out_of_range, "EnginioQmlModel::setProperty: row is out of range")\
    F(Dependent_create_query_failed_so_object_could_not_be_removed, "Dependent create query failed, so object could not be removed")\
    F(Dependent_create_query_failed_so_object_could_not_be_updated, "Dependent create query failed, so object could not be updated")\
    F(EnginioModel_was_removed_before_this_request_was_prepared, "EnginioModel was removed before this request was prepared")\
    F(EnginioModel_The_query_was_changed_before_the_request_could_be_sent, "EnginioModel: The query was changed before the request could be sent")\
    F(EnginioModel_Trying_to_update_an_object_with_unknown_role, "EnginioModel: Trying to update an object with unknown role")\
    F(Content_Range, "Content-Range")\
    F(Content_Type, "Content-Type")\
    F(Get, "GET")\
    F(Accept, "Accept")\
 

struct QTCLOUDSERVICES_EXPORT QtCloudServicesConstants {
#define DECLARE_ENGINIO_STRING(Name, String)\
    static const QString Name;

    FOR_EACH_QTCLOUDSERVICES_STRING(DECLARE_ENGINIO_STRING)
#undef DECLARE_ENGINIO_STRING


#define DECLARE_ENGINIO_BYTEARRAY(Name, String)\
    static const QByteArray Name;

    FOR_EACH_QTCLOUDSERVICES_BYTEARRAY(DECLARE_ENGINIO_BYTEARRAY)
#undef DECLARE_ENGINIO_BYTEARRAY
};


#endif /* QCLOUDSERVICES_QCLOUDSERVICES_CONSTANTS_P_H */
