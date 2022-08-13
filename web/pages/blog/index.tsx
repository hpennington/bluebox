
import type { NextPage } from 'next'
import Head from 'next/head'
import Image from 'next/image'
import {Header} from '../../components/header/header'
import {PostOverview} from '../../components/posts/post-overview'
import styles from './Blog.module.css'

const Blog: NextPage = () => {
  return (
    <div className={styles.container}>
      <Head>
        <title>Create Next App</title>
        <meta name="description" content="Generated by create next app" />
      </Head>

      <main className={styles.main}>
        <Header />
        <h1 className={styles.title}>Latest</h1>
        <p className={styles.description}>A blog about the development of a bluetooth guitar pedal. Also covers product & feature announcements.</p>
        
       <PostOverview title={'PWM DAC vs Discrete DAC'} overview={'This is an overview of the blog post you are previewing.'} slug={'blog/posts/PWM_DAC'} tag0={'AVR'} tag1={'SPI'} tag2={'PWM'} date={'Saturday, August 13, 2022'} />
       <PostOverview title={'ADC Overview'} overview={'This is an overview of the blog post you are previewing.'} slug={'blog/posts/ADC'} tag0={'AVR'} tag1={'SPI'} tag2={'PWM'} date={'Friday, August 12, 2022'} />
       <PostOverview title={'Effects Processing'} overview={'This is an overview of the blog post you are previewing.'} slug={'blog/posts/Processing'} tag0={'AVR'} tag1={'SPI'} tag2={'PWM'} date={'Thursday, August 11, 2022'} />
      </main>

      <footer className={styles.footer}>
      </footer>
    </div>
  )
}

export default Blog
