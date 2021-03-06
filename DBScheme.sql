USE [master]
GO
/****** Object:  Database [testDB]    Script Date: 2019-01-22 오전 11:13:54 ******/
CREATE DATABASE [testDB]
 CONTAINMENT = NONE
 ON  PRIMARY 
( NAME = N'testDB', FILENAME = N'C:\Program Files\Microsoft SQL Server\MSSQL14.MSSQLSERVER\MSSQL\DATA\testDB.mdf' , SIZE = 8192KB , MAXSIZE = UNLIMITED, FILEGROWTH = 65536KB )
 LOG ON 
( NAME = N'testDB_log', FILENAME = N'C:\Program Files\Microsoft SQL Server\MSSQL14.MSSQLSERVER\MSSQL\DATA\testDB_log.ldf' , SIZE = 8192KB , MAXSIZE = 2048GB , FILEGROWTH = 65536KB )
GO
ALTER DATABASE [testDB] SET COMPATIBILITY_LEVEL = 140
GO
IF (1 = FULLTEXTSERVICEPROPERTY('IsFullTextInstalled'))
begin
EXEC [testDB].[dbo].[sp_fulltext_database] @action = 'enable'
end
GO
ALTER DATABASE [testDB] SET ANSI_NULL_DEFAULT OFF 
GO
ALTER DATABASE [testDB] SET ANSI_NULLS OFF 
GO
ALTER DATABASE [testDB] SET ANSI_PADDING OFF 
GO
ALTER DATABASE [testDB] SET ANSI_WARNINGS OFF 
GO
ALTER DATABASE [testDB] SET ARITHABORT OFF 
GO
ALTER DATABASE [testDB] SET AUTO_CLOSE OFF 
GO
ALTER DATABASE [testDB] SET AUTO_SHRINK OFF 
GO
ALTER DATABASE [testDB] SET AUTO_UPDATE_STATISTICS ON 
GO
ALTER DATABASE [testDB] SET CURSOR_CLOSE_ON_COMMIT OFF 
GO
ALTER DATABASE [testDB] SET CURSOR_DEFAULT  GLOBAL 
GO
ALTER DATABASE [testDB] SET CONCAT_NULL_YIELDS_NULL OFF 
GO
ALTER DATABASE [testDB] SET NUMERIC_ROUNDABORT OFF 
GO
ALTER DATABASE [testDB] SET QUOTED_IDENTIFIER OFF 
GO
ALTER DATABASE [testDB] SET RECURSIVE_TRIGGERS OFF 
GO
ALTER DATABASE [testDB] SET  ENABLE_BROKER 
GO
ALTER DATABASE [testDB] SET AUTO_UPDATE_STATISTICS_ASYNC OFF 
GO
ALTER DATABASE [testDB] SET DATE_CORRELATION_OPTIMIZATION OFF 
GO
ALTER DATABASE [testDB] SET TRUSTWORTHY OFF 
GO
ALTER DATABASE [testDB] SET ALLOW_SNAPSHOT_ISOLATION OFF 
GO
ALTER DATABASE [testDB] SET PARAMETERIZATION SIMPLE 
GO
ALTER DATABASE [testDB] SET READ_COMMITTED_SNAPSHOT OFF 
GO
ALTER DATABASE [testDB] SET HONOR_BROKER_PRIORITY OFF 
GO
ALTER DATABASE [testDB] SET RECOVERY FULL 
GO
ALTER DATABASE [testDB] SET  MULTI_USER 
GO
ALTER DATABASE [testDB] SET PAGE_VERIFY CHECKSUM  
GO
ALTER DATABASE [testDB] SET DB_CHAINING OFF 
GO
ALTER DATABASE [testDB] SET FILESTREAM( NON_TRANSACTED_ACCESS = OFF ) 
GO
ALTER DATABASE [testDB] SET TARGET_RECOVERY_TIME = 60 SECONDS 
GO
ALTER DATABASE [testDB] SET DELAYED_DURABILITY = DISABLED 
GO
EXEC sys.sp_db_vardecimal_storage_format N'testDB', N'ON'
GO
ALTER DATABASE [testDB] SET QUERY_STORE = OFF
GO
USE [testDB]
GO
/****** Object:  Table [dbo].[testTable]    Script Date: 2019-01-22 오전 11:13:54 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[testTable](
	[username] [nvarchar](16) NOT NULL,
	[password] [binary](50) NOT NULL,
 CONSTRAINT [PK_testTable_1] PRIMARY KEY CLUSTERED 
(
	[username] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  StoredProcedure [dbo].[usp_DeleteAccount]    Script Date: 2019-01-22 오전 11:13:55 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[usp_DeleteAccount] 
	-- Add the parameters for the stored procedure here
	@pUsername nvarchar(16), 
	@pPassword nvarchar(16)
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;

    -- Insert statements for procedure here
	DELETE FROM dbo.testTable
	where username = @pUsername AND password = HASHBYTES('sha1', @pPassword)
END
GO
/****** Object:  StoredProcedure [dbo].[usp_SignIn]    Script Date: 2019-01-22 오전 11:13:55 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		pointerphile
-- Create date: 
-- Description:	
-- =============================================
CREATE PROCEDURE [dbo].[usp_SignIn] 
	-- Add the parameters for the stored procedure here
	@nstrUsername nvarchar(16), 
	@nstrPassword nvarchar(16),
	@iResult int OUTPUT
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;

    -- Insert statements for procedure here
	IF EXISTS(
			SELECT username, password
			FROM dbo.testTable
			where username = @nstrUsername AND password = HASHBYTES('sha1', @nstrPassword)
		)
		BEGIN
		SET @iResult = 0
		END
	ELSE
		BEGIN
		SET @iResult = -1
		END
END
GO
/****** Object:  StoredProcedure [dbo].[usp_SignUp]    Script Date: 2019-01-22 오전 11:13:55 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		pointerphile
-- Create date: 
-- Description:	
-- =============================================
CREATE PROCEDURE [dbo].[usp_SignUp] 
	-- Add the parameters for the stored procedure here
	@pUsername nvarchar(16), 
	@pPassword nvarchar(16)
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;

    -- Insert statements for procedure here
	INSERT INTO dbo.testTable
	VALUES (@pUsername, HASHBYTES('sha1', @pPassword))
END
GO
/****** Object:  StoredProcedure [dbo].[usp_UpdateUsername]    Script Date: 2019-01-22 오전 11:13:55 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[usp_UpdateUsername] 
	-- Add the parameters for the stored procedure here
	@nstrUsername nvarchar(16), 
	@nstrNewUsername nvarchar(16)
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;

    -- Insert statements for procedure here
	UPDATE [dbo].[testTable]
	SET username = @nstrNewUsername
	WHERE username = @nstrUsername;
END
GO
USE [master]
GO
ALTER DATABASE [testDB] SET  READ_WRITE 
GO
