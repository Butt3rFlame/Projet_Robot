/**
 * @file error.h
 *
 * Gestion d'erreur de l'API de programmation du MRPiZ pour le projet PATO.
 *
 * @version 0.1
 * @author Matthias Brun (matthias.brun@eseo.fr)
 *
 */

#ifndef MRPIZ_ERROR_H_
#define MRPIZ_ERROR_H_

/**
 * @defgroup mrpiz_error Gestion d'erreur pour la programmation d'un robot MRPiZ.
 *
 * @brief
 * Fonctions et macros utiles à la gestion d'erreur pour la programmation d'un MRPiZ.
 *
 * @{
 */

/**
 * @enum mrpiz_error_t
 *
 * @brief Liste des erreurs MRPiZ
 *
 * Code d'erreur utilisés pour renseigner errno lors de l'exécution des fonctions de la bibliothèque.
 *
 * Remarque : L'énumératin commence à 1 (errno n'est jamais mis à zero lorsqu'une erreur survient (man errno)).
 *
 */
typedef enum
{
	MRPIZ_OK 			= 1,	/**< Succès. */
	MRPIZ_E_INIT,				/**< MRPiZ n'est pas initialisé. */
	MRPIZ_E_MOTOR_CMD,			/**< Commande moteur invalide. */
	MRPIZ_E_MOTOR_ID,			/**< Identifiant de moteur invalide. */
	MRPIZ_E_PROXY_SENSOR_ID,	/**< Identifiant de capteur de proximité invalide. */

	// Erreurs spécifiques à MRPiZ :
	MRPIZ_E_UART,				/**< Erreur durant la communication UART entre la PiZ et la STM32.	*/

	// Erreurs spécifiques à Intox :

	MRPIZ_E_SOCKET,				/**< Erreur durant la création de la socket de connexion au simulateur. */
	MRPIZ_E_CONNECT,			/**< Erreur durant la connexion au simulateur Intox (erreur par défaut). */
	MRPIZ_E_CONNECT_REFUSED,	/**< Erreur durant la connexion au simulateur Intox : connexion refusée. */
	MRPIZ_E_CONNECT_NET,		/**< Erreur durant la connexion au simulateur Intox : réseau inaccessible. */
	MRPIZ_E_CONNECT_HOST,		/**< Erreur durant la connexion au simulateur Intox : serveur inaccessible. */
	MRPIZ_E_CONNECT_TIMEOUT,	/**< Erreur durant la connexion au simulateur Intox : échéance de connexion. */

	MRPIZ_E_INTOX, 				/**< Erreur d'accès au simulateur Intox. */
	MRPIZ_E_INTOX_LOST, 		/**< Perte d'accès au simulateur Intox. */
	MRPIZ_E_INTOX_CMD,	 		/**< Commande invalide au simulateur Intox. */


	MRPIZ_E_UNDEFINED			/**< Fin de la liste d'erreurs. */

} mrpiz_error_t;


/**
 * @fn char const * mrpiz_error_msg()
 *
 * @brief Donne le message d'erreur (associé à la valeur de errno de la lib mrpiz).
 *
 * @return le message d'erreur
 */
char const * mrpiz_error_msg();

/**
 * @fn void mrpiz_error_print(char * msg)
 *
 * @brief Affiche le message d'erreur (associé à la valeur errno de la lib mrpiz).
 *
 *
 * @param msg le préfixe du message à afficher (NULL si pas de préfixe)
 */
void mrpiz_error_print(char * msg);

/** @} */
// end of mrpiz_error group

#endif /* MRPIZ_ERROR_H_ */
