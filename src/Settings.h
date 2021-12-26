// <copyright company="Scythe Studio Sp. z o.o.">
//     This file contains proprietary code. Copyright (c) 2021. All rights reserved.
// </copyright>

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>

/*!
 * \brief Class representing settings storing all the meta information of the application.
 */
class Settings : public QObject
{
    Q_OBJECT
public:

    /*!
     * \brief Constructor.
     */
    explicit Settings(QObject *parent = nullptr);

    /*!
     * \brief Returns current access token.
     */
    QString accessToken() const;

    /*!
     * \brief Sets access token.
     * \param const QString &accessToken -- value to set.
     */
    void setAccessToken(const QString &accessToken);

    /*!
     * \brief Returns current refresh token.
     */
    QString refreshToken() const;

    /*!
     * \brief Sets access token.
     * \param const QString &refreshToken -- value to set.
     */
    void setRefreshToken(const QString &refreshToken);

    /*!
     * \brief Returns true if the application is currently in the dark mode. Otherwise returns false.
     */
    bool isDarkMode() const;

    /*!
     * \brief Sets dark mode state of the application.
     * \param isDarkMode - value to set.
     */
    void setIsDarkMode(bool isDarkMode);

    /*!
     * \brief Returns true if the application is currently in the demo mode. Otherwise returns false.
     */
    bool isDemoMode() const;

    /*!
     * \brief Sets demo mode state of the application.
     * \param isDemoMode - value to set.
     */
    void setIsDemoMode(bool isDemoMode);

private:
    QSettings *m_settings = nullptr;
};

#endif // SETTINGS_H
