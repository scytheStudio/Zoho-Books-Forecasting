// <copyright company="Scythe Studio Sp. z o.o.">
//     This file contains proprietary code. Copyright (c) 2021. All rights reserved.
// </copyright>

#include "Settings.h"
#include <QApplication>
#include <QDir>

#define ACCESS_TOKEN "accessToken"
#define REFRESH_TOKEN "refreshToken"
#define IS_DARK_MODE_ENABLED "isDarkModeEnabled"
#define IS_DEMO_MODE "isDemoMode"

Settings::Settings(QObject *parent)
    : QObject(parent)
    , m_settings(new QSettings("tokens.ini", QSettings::IniFormat, this))
{

}

QString Settings::accessToken() const
{
    return m_settings->value(ACCESS_TOKEN, "").toString();
}

void Settings::setAccessToken(const QString &accessToken)
{
    if (this->accessToken() != accessToken) {
        m_settings->setValue(ACCESS_TOKEN, accessToken);
    }
}

QString Settings::refreshToken() const
{
    return m_settings->value(REFRESH_TOKEN, "").toString();
}

void Settings::setRefreshToken(const QString &refreshToken)
{
    if (this->refreshToken() != refreshToken)
    {
        m_settings->setValue(REFRESH_TOKEN, refreshToken);
    }
}

bool Settings::isDarkMode() const
{
    return m_settings->value(IS_DARK_MODE_ENABLED, false).toBool();
}

void Settings::setIsDarkMode(bool isDarkMode)
{
    m_settings->setValue(IS_DARK_MODE_ENABLED, isDarkMode);
}

bool Settings::isDemoMode() const
{
    return m_settings->value(IS_DEMO_MODE, false).toBool();
}

void Settings::setIsDemoMode(bool value)
{
    m_settings->setValue(IS_DEMO_MODE, value);
}
